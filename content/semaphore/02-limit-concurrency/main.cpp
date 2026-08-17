#include <atomic>
#include <iostream>
#include <semaphore>
#include <thread>
#include <vector>

// A std::counting_semaphore<Max> holds up to Max permits. Start it with K permits
// and at most K threads can hold one at a time: acquire() blocks while the count is
// 0, release() frees a permit for the next waiter. This is the standard way to cap
// how many threads run a section in parallel -- a connection pool, a rate limit, a
// fixed set of slots. A plain mutex cannot do this: a mutex caps you at ONE.
int main() {
    const int LIMIT = 3;
    std::counting_semaphore<3> slots{LIMIT};   // 3 permits -> at most 3 inside

    std::atomic<int> inside{0};        // how many are in the region right now
    std::atomic<int> peak{0};          // the most we ever saw at once

    std::vector<std::jthread> workers;
    for (int i = 0; i < 12; ++i) {
        workers.emplace_back([&] {
            slots.acquire();           // wait for a free permit
            int now = ++inside;        // entered the limited region
            int seen = peak.load();    // record a new high-water mark if we set one
            while (now > seen && !peak.compare_exchange_weak(seen, now)) {}
            // ... do the limited work ...
            --inside;
            slots.release();           // give the permit back for the next waiter
        });
    }
    workers.clear();                   // join every worker (jthread destructor)

    // The semaphore never let more than LIMIT threads hold a permit at once.
    std::cout << "peak concurrent <= limit (" << LIMIT << ") = "
              << std::boolalpha << (peak.load() <= LIMIT) << '\n';   // true
    return 0;
}
