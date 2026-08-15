#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    // ANTI-PATTERN: a non-atomic "run once" guard.
    //
    //   bool done = false;
    //   int initCount = 0;
    //   // each thread:
    //   if (!done) { done = true; ++initCount; }   // do the init
    //
    // The check and the set are two separate steps on a plain bool. Several
    // threads can all read done == false before any of them writes true, so the
    // body runs MORE THAN ONCE -- and the concurrent reads/writes of `done` are a
    // data race (undefined behaviour) that ThreadSanitizer flags at once. We do
    // not run that version.

    // FIX: exchange makes test-and-set a single atomic step. Exactly one thread
    // sees the old value false and runs the body; the rest see true and skip.
    std::atomic<bool> done{false};
    std::atomic<int> initCount{0};
    std::vector<std::thread> pool;
    for (int i = 0; i < 8; ++i)
        pool.emplace_back([&] {
            if (!done.exchange(true)) {          // atomic test-and-set
                initCount.fetch_add(1);          // the "init", run once
            }
        });
    for (auto& t : pool) t.join();

    std::cout << "init ran " << initCount.load() << " time(s)\n";   // exactly 1
    // For a real one-time initialization, std::call_once with a std::once_flag is
    // the purpose-built tool and also handles exceptions during init.
    return 0;
}
