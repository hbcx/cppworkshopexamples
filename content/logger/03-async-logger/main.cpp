#include "log.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>
#include <vector>

// A sink that only counts records. Because the async logger has exactly one
// worker thread writing to the sinks, a sink is never touched concurrently, so
// it needs no lock of its own -- the queue already serialised everything.
class CountingSink : public LogSink {
public:
    void write(const LogRecord&) override { ++count_; }
    long long count() const { return count_; }

private:
    long long count_ = 0;
};

int main() {
    // Part 1: one thread logs a few records; the WORKER thread does the writing.
    // Single producer, so the sequence numbers and order are deterministic.
    {
        std::ostringstream file;
        // Deterministic clock (single producer here) so the output is reproducible.
        using namespace std::chrono;
        AsyncLogger logger(64, {std::make_shared<StreamSink>(file)},
                           StepClock{sys_days{year{2026} / 8 / 21} + 14h + 5min, 250ms});
        log<Level::Info, Severity::Minor, Component::General>(logger, "startup complete");
        log<Level::Info, Severity::Minor, Component::Network>(logger, "bound to port {}", 8080);
        log<Level::Error, Severity::Critical, Component::Storage>(logger, "write failed: {}", "ENOSPC");
        logger.shutdown(); // drains the queue, then joins the worker
        std::cout << "--- written by the background thread ---\n" << file.str();
    }

    // Part 2: many threads log at once. They all push onto the one queue and the
    // single worker writes them, so the ORDER varies run to run -- but the COUNT
    // does not, which is what we check (a deterministic result over a concurrent
    // run, verified clean by ThreadSanitizer).
    {
        const int threads = 4;
        const int perThread = 500;
        auto counter = std::make_shared<CountingSink>();
        AsyncLogger logger(128, {counter});

        std::vector<std::thread> producers;
        for (int t = 0; t < threads; ++t) {
            producers.emplace_back([&logger, t] {
                for (int i = 0; i < perThread; ++i)
                    log<Level::Info, Severity::Minor, Component::General>(
                        logger, "event {} from worker {}", i, t);
            });
        }
        for (auto& p : producers) p.join(); // all records enqueued
        logger.shutdown();                  // worker drains them all, then stops

        const long long expected = 1LL * threads * perThread;
        std::cout << "\n--- concurrent logging ---\n";
        std::cout << "records delivered: " << counter->count() << " of " << expected
                  << (counter->count() == expected ? "  OK\n" : "  MISMATCH\n");
    }
    return 0;
}
