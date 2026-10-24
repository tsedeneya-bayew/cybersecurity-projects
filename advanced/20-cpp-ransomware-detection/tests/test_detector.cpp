#include "detector.hpp"
#include <iostream>
#include <thread>
void check(bool condition, const char* name) {
    if (!condition) throw std::runtime_error(name);
}
int main() {
    try {
        lab::Incident incident{};
        lab::Detector normal;
        for (int i = 0; i < 50; ++i)
            check(!normal.process({i * 0.01, "modified", "one.txt"}, incident), "Repeated single file");
        lab::Detector burst({10, 3, 10});
        check(!burst.process({0, "modified", "a"}, incident), "Below threshold");
        check(!burst.process({1, "modified", "b"}, incident), "Below threshold 2");
        check(burst.process({2, "modified", "c"}, incident) && incident.score == 2, "Distinct burst");
        check(!burst.process({3, "modified", "d"}, incident), "Cooldown");
        check(burst.process({4, "modified", "CANARY_DO_NOT_EDIT.txt"}, incident)
            && incident.score == 6, "Escalation bypasses cooldown");
        lab::Detector expiry({1, 3, 0});
        expiry.process({0, "modified", "a"}, incident);
        expiry.process({2, "modified", "b"}, incident);
        check(!expiry.process({2.5, "modified", "c"}, incident), "Window expiry");
        lab::Detector canary;
        check(!canary.process({0, "created", "CANARY_DO_NOT_EDIT.txt"}, incident), "New canary");
        check(canary.process({1, "deleted", "CANARY_DO_NOT_EDIT.txt"}, incident), "Canary deleted");
        std::vector<unsigned char> bytes;
        for (int repeat = 0; repeat < 4; ++repeat)
            for (int i = 0; i < 256; ++i) bytes.push_back(static_cast<unsigned char>(i));
        check(std::abs(lab::entropy(bytes) - 8.0) < 1e-9, "Uniform entropy");
        check(lab::entropy({0,0,0}) == 0 && lab::entropy({}) == 0, "Low entropy");
        lab::Detector entropy_only;
        for (int i = 0; i < 3; ++i)
            check(!entropy_only.process({static_cast<double>(i), "modified", std::to_string(i), 1, 8}, incident),
                  "Entropy alone must not alert");
        lab::Detector pairs({10, 3, 0});
        bool alerted = false;
        for (int i = 0; i < 3; ++i) {
            pairs.process({0, "created", std::to_string(i)+".demo"}, incident);
            alerted = pairs.process({0, "deleted", std::to_string(i)}, incident) || alerted;
        }
        check(alerted && incident.score == 4, "Delete/create pairs plus burst");
        check(lab::json("a\n\"\\") == "\"a\\n\\\"\\\\\"", "JSON escape");
        check(lab::json(std::string(1, static_cast<char>(255))) == "\"\\u00ff\"", "Byte escaping");
        lab::Queue<int> queue(2);
        queue.push(1); queue.push(2);
        bool overflow = false;
        try { queue.push(3); } catch (const std::runtime_error&) { overflow = true; }
        check(overflow, "Queue overflow visible");
        queue.close();
        int value = 0;
        check(queue.pop(value) && value == 1, "Queue drain 1");
        check(queue.pop(value) && value == 2 && !queue.pop(value), "Queue drain 2 and close");
        lab::Queue<int> threaded;
        int sum = 0;
        std::thread consumer([&]{ int n; while (threaded.pop(n)) sum += n; });
        for (int i = 0; i < 100; ++i) threaded.push(i);
        threaded.close(); consumer.join();
        check(sum == 4950, "Concurrent queue");
        bool invalid = false;
        try { lab::Detector bad({0, 1, 0}); } catch (const std::invalid_argument&) { invalid = true; }
        check(invalid, "Invalid config");
        invalid = false;
        try { normal.process({-1, "modified", "x"}, incident); }
        catch (const std::invalid_argument&) { invalid = true; }
        check(invalid, "Out-of-order event rejected");
        std::cout << "PASS: detector thresholds, distinct counts, expiry, cooldown, escalation, canaries,\n"
            "entropy, extension correlation, JSON escaping, queue overflow/drain/concurrency, invalid inputs\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << "FAIL: " << e.what() << '\n'; return 1; }
}
