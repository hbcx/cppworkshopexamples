#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    // ANTI-PATTERN: build a read-modify-write out of a separate load and store.
    //
    //   counter.store(counter.load() + 1);
    //
    // Each of load() and store() IS atomic, which fools people into thinking the
    // whole thing is safe. It is not: there is a gap between the load and the
    // store, and another thread can increment in that gap, so its update is
    // overwritten and lost. The atomics prevent torn reads, not lost updates.
    // This is a lost-update bug (not flagged by ThreadSanitizer, because each
    // access really is atomic) -- it just produces a wrong total. We do not use it.

    // FIX: a single read-modify-write op. fetch_add does the read, the add, and
    // the store as ONE indivisible step, so nothing can slip into a gap.
    std::atomic<long> counter{0};
    const int threads = 4, perThread = 100000;
    std::vector<std::thread> pool;
    for (int i = 0; i < threads; ++i)
        pool.emplace_back([&] {
            for (int k = 0; k < perThread; ++k)
                counter.fetch_add(1);          // atomic RMW, no gap
        });
    for (auto& t : pool) t.join();

    std::cout << "counter = " << counter.load() << '\n';   // 400000, exact
    // Anything that must read, compute, and store as a unit needs fetch_* or a
    // compare_exchange loop -- never a load() followed by a separate store().
    return 0;
}
