#include "queue.hpp"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// Demo: several producers push numbers into one bounded queue while several
// consumers pull them out. The queue is small on purpose, so producers really
// do block on a full queue (backpressure) partway through.
//
// The output is a deterministic summary, not a log of who did what: the count
// and the sum of all values do not depend on the thread interleaving, so the
// program prints the same thing on every run and every compiler.
int main() {
    const int producers = 4;
    const int consumers = 3;
    const int perProducer = 1000;

    BoundedBlockingQueue<int> queue(64);
    std::atomic<long long> sum{0};
    std::atomic<int> consumed{0};

    std::vector<std::thread> consumerThreads;
    for (int c = 0; c < consumers; ++c) {
        consumerThreads.emplace_back([&] {
            // Runs until the queue is closed and drained (pop returns nullopt).
            while (std::optional<int> item = queue.pop()) {
                sum += *item;
                consumed.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    std::vector<std::thread> producerThreads;
    for (int p = 0; p < producers; ++p) {
        producerThreads.emplace_back([&] {
            for (int i = 1; i <= perProducer; ++i)
                queue.push(i);
        });
    }

    for (auto& t : producerThreads) t.join(); // every item is now enqueued
    queue.close();                            // let consumers drain, then stop
    for (auto& t : consumerThreads) t.join();

    const int expectedCount = producers * perProducer;
    const long long expectedSum =
        1LL * producers * perProducer * (perProducer + 1) / 2;

    std::cout << "produced items: " << expectedCount << '\n';
    std::cout << "consumed items: " << consumed.load() << '\n';
    std::cout << "sum of values:  " << sum.load() << '\n';
    std::cout << "expected sum:   " << expectedSum << '\n';
    std::cout << "result: "
              << (consumed.load() == expectedCount && sum.load() == expectedSum
                      ? "OK"
                      : "MISMATCH")
              << '\n';
    return 0;
}
