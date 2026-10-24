#include "detector.hpp"
#include <atomic>
#include <chrono>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <thread>

namespace fs = std::filesystem;
using Clock = std::chrono::steady_clock;
static volatile std::sig_atomic_t interrupted = 0;
void stop_handler(int) { interrupted = 1; }

struct Sample {
    std::uintmax_t size;
    fs::file_time_type modified;
    std::uint64_t fingerprint;
    double entropy;
};
using Snapshot = std::map<std::string, Sample>;
Snapshot snapshot(const fs::path& root) {
    Snapshot result;
    // Default traversal does not follow directory symlinks. Skip file links too.
    for (const auto& item : fs::recursive_directory_iterator(root)) {
        if (item.is_symlink() || !item.is_regular_file()) continue;
        if (result.size() >= 10000) throw std::runtime_error("Lab limit: 10000 files");
        auto size = item.file_size();
        auto modified = item.last_write_time();
        std::ifstream input(item.path(), std::ios::binary);
        if (!input) throw std::runtime_error("Cannot sample file: " + item.path().string());
        std::vector<unsigned char> bytes(65536);
        input.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        if (input.bad()) throw std::runtime_error("File sample read failed");
        bytes.resize(static_cast<std::size_t>(input.gcount()));
        // FNV-1a is a change fingerprint, NOT a cryptographic integrity hash.
        std::uint64_t hash = 14695981039346656037ULL;
        for (auto b : bytes) { hash ^= b; hash *= 1099511628211ULL; }
        auto relative = item.path().lexically_relative(root).generic_string();
        result.emplace(relative, Sample{size, modified, hash, lab::entropy(bytes)});
    }
    return result;
}

bool inside(const fs::path& path, const fs::path& root) {
    auto p = path.begin();
    for (auto r = root.begin(); r != root.end(); ++r, ++p)
        if (p == path.end() || *p != *r) return false;
    return true;
}

int main(int argc, char** argv) {
    if (argc < 3 || argc > 5) {
        std::cerr << "Usage: monitor WATCH_FOLDER NEW_REPORT_FOLDER [seconds=60] [poll_ms=250]\n";
        return 1;
    }
    try {
        const int seconds = argc > 3 ? std::stoi(argv[3]) : 60;
        const int poll_ms = argc > 4 ? std::stoi(argv[4]) : 250;
        if (seconds < 1 || seconds > 86400 || poll_ms < 50 || poll_ms > 10000)
            throw std::runtime_error("Seconds must be 1..86400; poll_ms must be 50..10000");
        if (fs::is_symlink(fs::path(argv[1]))) throw std::runtime_error("Root cannot be a symlink");
        const auto root = fs::canonical(argv[1]);
        if (!fs::is_directory(root)) throw std::runtime_error("Watch folder is not a directory");
        const auto report = fs::weakly_canonical(fs::absolute(argv[2]));
        if (inside(report, root) || inside(root, report))
            throw std::runtime_error("Watch and report folders must be separate, nonnested folders");
        // Require a NEW output folder: existing evidence is never overwritten.
        if (!fs::create_directory(report)) throw std::runtime_error("Report folder must not exist");
        std::ofstream events(report / "events.jsonl"), incidents(report / "incidents.jsonl");
        if (!events || !incidents) throw std::runtime_error("Cannot create evidence logs");
        auto previous = snapshot(root);
        std::cout << "BASELINE READY files=" << previous.size() << " backend=polling\n" << std::flush;
        const auto start = Clock::now();
        const auto deadline = start + std::chrono::seconds(seconds);
        std::signal(SIGINT, stop_handler);
        std::atomic<bool> failed{false};
        lab::Queue<lab::Event> queue;
        std::exception_ptr worker_error;
        std::size_t event_count = 0, incident_count = 0;
        std::thread worker([&] {
            try {
                lab::Detector detector;
                lab::Event event{};
                while (queue.pop(event)) {
                    events << lab::event_json(event) << '\n' << std::flush;
                    if (!events) throw std::runtime_error("Event log write failed");
                    ++event_count;
                    lab::Incident incident{};
                    if (!detector.process(event, incident)) continue;
                    ++incident_count;
                    const auto epoch_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count();
                    incidents << "{\"id\":" << incident_count << ",\"recorded_unix_ms\":" << epoch_ms
                        << ",\"elapsed_seconds\":" << event.time << ",\"score\":" << incident.score
                        << ",\"severity\":" << lab::json(incident.score >= 4 ? "high" : "medium")
                        << ",\"signals\":[";
                    for (std::size_t i = 0; i < incident.signals.size(); ++i) {
                        if (i) incidents << ',';
                        incidents << lab::json(incident.signals[i]);
                    }
                    incidents << "],\"evidence\":[";
                    for (std::size_t i = 0; i < incident.evidence.size(); ++i) {
                        if (i) incidents << ',';
                        incidents << lab::event_json(incident.evidence[i]);
                    }
                    incidents << "]}\n" << std::flush;
                    if (!incidents) throw std::runtime_error("Incident log write failed");
                    std::cout << "ALERT id=" << incident_count << " score=" << incident.score
                        << " latest=" << lab::json(event.path) << '\n' << std::flush;
                }
            } catch (...) { worker_error = std::current_exception(); failed = true; }
        });
        std::exception_ptr collector_error;
        try {
            while (!interrupted && !failed && Clock::now() < deadline) {
                std::this_thread::sleep_for(std::chrono::milliseconds(poll_ms));
                auto current = snapshot(root);
                const double now = std::chrono::duration<double>(Clock::now() - start).count();
                for (const auto& pair : current) {
                    const auto old = previous.find(pair.first);
                    if (old == previous.end())
                        queue.push({now, "created", pair.first, 0, pair.second.entropy});
                    else if (old->second.size != pair.second.size || old->second.modified != pair.second.modified ||
                             old->second.fingerprint != pair.second.fingerprint)
                        queue.push({now, "modified", pair.first, old->second.entropy, pair.second.entropy});
                }
                for (const auto& pair : previous)
                    if (!current.count(pair.first))
                        queue.push({now, "deleted", pair.first, pair.second.entropy, 0});
                previous = std::move(current);
            }
        } catch (...) { collector_error = std::current_exception(); }
        queue.close();
        worker.join(); // Drain queued evidence before exit, including Ctrl+C.
        if (collector_error) std::rethrow_exception(collector_error);
        if (worker_error) std::rethrow_exception(worker_error);
        std::ofstream summary(report / "summary.json");
        summary << "{\"status\":\"complete\",\"backend\":\"polling\",\"events\":" << event_count
            << ",\"incidents\":" << incident_count << ",\"interrupted\":"
            << (interrupted ? "true" : "false") << "}\n";
        summary.close();
        if (!summary) throw std::runtime_error("Summary write failed");
        std::cout << "STOPPED events=" << event_count << " incidents=" << incident_count << '\n';
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\nMonitoring incomplete; inspect stderr and partial logs.\n";
        return 2;
    }
}
