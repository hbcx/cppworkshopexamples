#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
    // A std::mutex gives threads MUTUAL EXCLUSION: at most one thread can hold it at
    // a time, so the code between locking and unlocking -- the critical section --
    // runs without another thread interleaving. The previous example did that with
    // raw lock()/unlock(); std::lock_guard replaces that pair with RAII -- it locks
    // the mutex in its constructor and unlocks in its destructor, so you cannot
    // forget to unlock and the mutex is released even if the scope is left early by
    // an exception. This is how you normally take a mutex.
    long counter = 0;                 // a plain long, but only ever touched under 'm'
    std::mutex m;

    std::vector<std::thread> pool;
    for (int i = 0; i < 4; ++i)
        pool.emplace_back([&] {
            for (int k = 0; k < 100000; ++k) {
                std::lock_guard<std::mutex> guard(m);   // lock now; unlock at scope end
                ++counter;                              // protected: one thread at a time
            }
        });
    for (auto& t : pool) t.join();

    std::cout << "counter = " << counter << '\n';   // 400000, no lost updates

    // Note: for a single counter a std::atomic is cheaper than a mutex (see the
    // atomic section). A mutex earns its cost when the critical section covers
    // SEVERAL variables that must change together -- the next example.
    return 0;
}
