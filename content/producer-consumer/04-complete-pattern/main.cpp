#include "queue.hpp"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// The full pattern, wired together: a three-stage pipeline joined by two bounded
// queues.
//
//   ingest  --stage1-->  processing workers  --stage2-->  collector
//
// One thread ingests raw items; several worker threads transform each item
// (here: square it) and forward the result; one collector sums the results.
// The bounded queues apply backpressure in BOTH directions -- if the collector
// is slow, stage2 fills and the workers block; if the workers are slow, stage1
// fills and the ingest thread blocks -- so memory stays bounded no matter which
// stage is the bottleneck.
//
// Shutdown is ordered, and that order is the whole trick: close a queue only
// once the stage feeding it has fully finished, so nothing in flight is lost.
int main() {
    const int itemCount = 5000;
    const int workerCount = 4;

    BoundedBlockingQueue<int> stage1(64);       // raw items: ingest -> workers
    BoundedBlockingQueue<long long> stage2(64); // results:   workers -> collector

    // Stage 3: the collector sums every processed result.
    std::atomic<long long> total{0};
    std::atomic<int> collected{0};
    std::thread collector([&] {
        while (std::optional<long long> result = stage2.pop()) {
            total += *result;
            collected.fetch_add(1, std::memory_order_relaxed);
        }
    });

    // Stage 2: workers transform each raw item and forward it.
    std::vector<std::thread> workers;
    for (int w = 0; w < workerCount; ++w) {
        workers.emplace_back([&] {
            while (std::optional<int> item = stage1.pop()) {
                long long processed = 1LL * *item * *item; // the "work"
                stage2.push(processed);
            }
        });
    }

    // Stage 1: ingest raw items.
    std::thread ingest([&] {
        for (int i = 1; i <= itemCount; ++i)
            stage1.push(i);
    });

    // Ordered, no-loss shutdown, front to back:
    ingest.join();                       // 1. all raw items are in stage1
    stage1.close();                      // 2. workers drain stage1, then exit
    for (std::thread& w : workers) w.join();
    stage2.close();                      // 3. collector drains stage2, then exits
    collector.join();

    long long expected = 0;
    for (int i = 1; i <= itemCount; ++i)
        expected += 1LL * i * i;

    std::cout << "items ingested:  " << itemCount << '\n';
    std::cout << "items collected: " << collected.load() << '\n';
    std::cout << "sum of squares:  " << total.load() << '\n';
    std::cout << "expected:        " << expected << '\n';
    std::cout << "result: "
              << (collected.load() == itemCount && total.load() == expected ? "OK"
                                                                            : "MISMATCH")
              << '\n';
    return 0;
}
