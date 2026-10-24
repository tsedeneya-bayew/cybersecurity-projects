#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <memory>
#include <mutex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace lab {
inline std::string json(const std::string& value) {
    std::ostringstream out;
    out << '"';
    for (unsigned char c : value) {
        switch (c) {
        case '"': out << "\\\""; break;
        case '\\': out << "\\\\"; break;
        case '\n': out << "\\n"; break;
        case '\r': out << "\\r"; break;
        case '\t': out << "\\t"; break;
        default:
            // Escape high bytes too: valid JSON even for non-UTF-8 filenames.
            if (c < 32 || c >= 127)
                out << "\\u00" << std::hex << std::setw(2)
                    << std::setfill('0') << static_cast<unsigned>(c) << std::dec;
            else out << static_cast<char>(c);
        }
    }
    return out.str() + '"';
}

inline double entropy(const std::vector<unsigned char>& bytes) {
    if (bytes.empty()) return 0;
    std::array<std::size_t, 256> counts{};
    for (auto b : bytes) ++counts[b];
    double result = 0;
    for (auto n : counts) if (n) {
        double p = static_cast<double>(n) / bytes.size();
        result -= p * std::log2(p);
    }
    return result;
}

struct Event {
    double time; // seconds since monitor start, from steady_clock
    std::string kind;
    std::string path; // relative to monitored root; no content in logs
    double before_entropy = 0;
    double after_entropy = 0;
};
struct Signal { std::string name; int weight; };
struct Config {
    double window_seconds = 10;
    std::size_t burst_files = 20;
    double cooldown_seconds = 10;
};

// Polymorphic rules keep evidence collection separate from detection policy.
class Rule {
public:
    virtual ~Rule() = default;
    virtual Signal evaluate(const std::deque<Event>& events, const Config& config) const = 0;
};
class BurstRule final : public Rule {
public:
    Signal evaluate(const std::deque<Event>& events, const Config& config) const override {
        std::set<std::string> paths;
        for (const auto& e : events)
            if (e.kind == "modified" || e.kind == "deleted") paths.insert(e.path);
        return {"many_distinct_files_changed", paths.size() >= config.burst_files ? 2 : 0};
    }
};
class CanaryRule final : public Rule {
public:
    Signal evaluate(const std::deque<Event>& events, const Config&) const override {
        for (const auto& e : events)
            if (e.path == "CANARY_DO_NOT_EDIT.txt" && e.kind != "created")
                return {"canary_changed", 4};
        return {"canary_changed", 0};
    }
};
class EntropyRule final : public Rule {
public:
    Signal evaluate(const std::deque<Event>& events, const Config&) const override {
        std::set<std::string> paths;
        for (const auto& e : events)
            if (e.kind == "modified" && e.after_entropy >= 7.5 &&
                e.after_entropy - e.before_entropy >= 1.5) paths.insert(e.path);
        return {"entropy_rise_in_multiple_files", paths.size() >= 3 ? 1 : 0};
    }
};
class ExtensionRule final : public Rule {
public:
    Signal evaluate(const std::deque<Event>& events, const Config& config) const override {
        std::set<std::string> created, deleted;
        for (const auto& e : events) {
            if (e.kind == "created" && e.path.size() >= 5 &&
                e.path.compare(e.path.size()-5, 5, ".demo") == 0) created.insert(e.path);
            if (e.kind == "deleted") deleted.insert(e.path);
        }
        std::size_t pairs = 0;
        for (const auto& path : created)
            if (deleted.count(path.substr(0, path.size()-5))) ++pairs;
        return {"demo_extension_delete_create_pairs", pairs >= config.burst_files ? 2 : 0};
    }
};

struct Incident {
    int score;
    std::vector<std::string> signals;
    std::deque<Event> evidence;
};
class Detector {
    Config config_;
    std::deque<Event> events_;
    std::vector<std::unique_ptr<Rule>> rules_;
    double last_alert_ = -1e30;
    int last_score_ = 0;
public:
    explicit Detector(Config config = {}) : config_(config) {
        if (config.window_seconds <= 0 || config.burst_files == 0 || config.cooldown_seconds < 0)
            throw std::invalid_argument("Invalid detection configuration");
        rules_.emplace_back(std::make_unique<BurstRule>());
        rules_.emplace_back(std::make_unique<CanaryRule>());
        rules_.emplace_back(std::make_unique<EntropyRule>());
        rules_.emplace_back(std::make_unique<ExtensionRule>());
    }
    // Caller submits events in nondecreasing monotonic time order.
    bool process(const Event& e, Incident& incident) {
        if (!events_.empty() && e.time < events_.back().time)
            throw std::invalid_argument("Out-of-order event");
        while (!events_.empty() && e.time - events_.front().time > config_.window_seconds)
            events_.pop_front();
        // Bound evidence memory. Overflow is an explicit failure, never silent loss.
        if (events_.size() >= 100000) throw std::runtime_error("Detection window capacity exceeded");
        events_.push_back(e);
        int score = 0;
        std::vector<std::string> names;
        for (const auto& rule : rules_) {
            auto signal = rule->evaluate(events_, config_);
            score += signal.weight;
            if (signal.weight) names.push_back(signal.name);
        }
        if (score < 2) { last_score_ = 0; return false; }
        if (e.time - last_alert_ < config_.cooldown_seconds && score <= last_score_) return false;
        last_alert_ = e.time;
        last_score_ = score;
        incident = {score, names, events_};
        return true;
    }
};

template<class T> class Queue {
    std::mutex mutex_;
    std::condition_variable ready_;
    std::deque<T> items_;
    std::size_t capacity_;
    bool closed_ = false;
public:
    explicit Queue(std::size_t capacity = 8192) : capacity_(capacity) {}
    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (closed_) throw std::runtime_error("Queue closed");
        if (items_.size() >= capacity_) throw std::runtime_error("Event queue capacity exceeded");
        items_.push_back(std::move(value));
        ready_.notify_one();
    }
    bool pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        ready_.wait(lock, [&]{ return closed_ || !items_.empty(); });
        if (items_.empty()) return false;
        value = std::move(items_.front()); items_.pop_front(); return true;
    }
    void close() {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true; ready_.notify_all();
    }
};

inline std::string event_json(const Event& e) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(6)
        << "{\"elapsed_seconds\":" << e.time << ",\"kind\":" << json(e.kind)
        << ",\"path\":" << json(e.path) << ",\"entropy_before\":" << e.before_entropy
        << ",\"entropy_after\":" << e.after_entropy << '}';
    return out.str();
}
} // namespace lab
