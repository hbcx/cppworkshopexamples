#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// std::call_once with a std::once_flag runs an initializer EXACTLY once, even
// when many threads reach it at the same time. The first thread to arrive runs
// the function; every other thread blocks until that run finishes, then skips
// it. This is the purpose-built tool for thread-safe lazy initialization -- a
// config loaded on first use, a singleton, a connection opened once -- and it is
// the correct fix for the racy "check a bool, then set it" run-once guard.
//
// It gives you two guarantees a hand-rolled atomic flag does not: other threads
// WAIT until the initializer has finished (so no one sees a half-built object),
// and if the initializer THROWS, the flag stays unset so the next caller retries.

std::once_flag config_flag;
int config = 0;                          // the state we initialize exactly once
std::atomic<int> init_runs{0};           // how many times the initializer ran

void init_config() {
    init_runs.fetch_add(1);
    config = 42;                         // stand-in for expensive one-time setup
}

int main() {
    std::atomic<long> total{0};
    std::vector<std::thread> pool;
    for (int i = 0; i < 8; ++i) {
        pool.emplace_back([&] {
            // All 8 threads call this; only the first actually runs init_config,
            // and all of them block until it has completed before continuing.
            std::call_once(config_flag, init_config);
            total.fetch_add(config);     // config is fully initialized here
        });
    }
    for (auto& t : pool) t.join();

    std::cout << "init ran " << init_runs.load() << " time(s)\n";        // 1
    std::cout << "8 threads each saw config, total = " << total.load()
              << '\n';                                                    // 336
    return 0;
}
