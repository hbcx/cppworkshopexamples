#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    // RELAXED: atomic, but with NO ordering guarantees relative to other memory.
    // That is fine for a standalone counter, where we only need each increment to
    // be indivisible and do not publish any other data through it.
    std::atomic<long> hits{0};
    std::vector<std::thread> pool;
    for (int i = 0; i < 4; ++i)
        pool.emplace_back([&] {
            for (int k = 0; k < 100000; ++k)
                hits.fetch_add(1, std::memory_order_relaxed);
        });
    for (auto& t : pool) t.join();
    std::cout << "relaxed counter = " << hits.load() << '\n';   // 400000

    // ACQUIRE / RELEASE: the pattern for PUBLISHING data. The producer writes a
    // plain payload, then stores the flag with release; the consumer waits on the
    // flag with acquire, then reads the payload. The release-acquire pair creates a
    // happens-before edge, so the payload write is guaranteed visible -- no race.
    int payload = 0;                          // plain, non-atomic
    std::atomic<bool> ready{false};

    std::thread producer([&] {
        payload = 42;                                        // (1) write data
        ready.store(true, std::memory_order_release);        // (2) publish
    });
    std::thread consumer([&] {
        while (!ready.load(std::memory_order_acquire)) {     // (3) wait for publish
        }
        std::cout << "handed over payload = " << payload << '\n';  // (4) sees 42
    });
    producer.join();
    consumer.join();

    // Using relaxed on the flag instead of release/acquire would break the
    // handoff: the consumer could observe ready == true before the payload write
    // is visible, reading a stale 0 -- a data race (see the anti-pattern).
    return 0;
}
