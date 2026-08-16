#include <future>
#include <iostream>
#include <thread>
#include <vector>

// A plain std::future is move-only and get() may be called only once -- it moves
// the result out, so it fits exactly one consumer. When MANY threads must all
// see the same one-shot result, use std::shared_future: it is copyable, and
// every copy can call get() to read the same value (get() returns a const
// reference and does not consume it). This is a broadcast: compute once, let all
// waiters observe it. Here one configuration value is produced, then five
// workers each read it through their own copy of the shared_future.
int main() {
    std::promise<long> config;
    // .share() turns the single future into a shared_future that can be copied
    // to every waiter.
    std::shared_future<long> ready = config.get_future().share();

    const int N = 5;
    std::vector<long> slots(N, 0);      // per-worker slot: no shared write
    std::vector<std::thread> workers;
    for (int i = 0; i < N; ++i) {
        workers.emplace_back([ready, &slots, i] {   // each captures its OWN copy
            long value = ready.get();   // all block here; all see the same value
            slots[i] = value;
        });
    }

    // Produce the value once; the single set_value releases every waiter.
    config.set_value(100);

    for (auto& w : workers) w.join();

    long total = 0;
    for (long s : slots) total += s;    // 5 workers each read 100
    std::cout << "workers = " << N << ", total = " << total << '\n';   // 500
    return 0;
}
