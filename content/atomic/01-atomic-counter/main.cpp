#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    // A std::atomic<long> shared by several threads. fetch_add performs an
    // indivisible read-modify-write: no two threads can interleave inside a single
    // increment, so no updates are lost. The plain ++ on a non-atomic long would
    // be a data race (that is the first anti-pattern).
    std::atomic<long> counter{0};

    const int threads = 4;
    const int perThread = 100000;
    std::vector<std::thread> pool;
    for (int i = 0; i < threads; ++i)
        pool.emplace_back([&] {
            for (int k = 0; k < perThread; ++k)
                counter.fetch_add(1);        // same as ++counter on an atomic
        });
    for (auto& t : pool) t.join();

    // The total is exact and deterministic: every increment counted once.
    std::cout << "counter = " << counter.load() << '\n';         // 400000
    std::cout << "expected = " << (threads * perThread) << '\n';
    return 0;
}
