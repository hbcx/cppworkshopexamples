#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    // ANTI-PATTERN: volatile for thread communication.
    //
    //   volatile long counter = 0;
    //   // each thread: for (...) ++counter;
    //
    // volatile means "do not optimize away this access" -- it was designed for
    // memory-mapped hardware registers, NOT for threads. It gives neither
    // atomicity (++counter is still load/add/store, so increments are lost) nor
    // any cross-thread ordering or visibility guarantee. Using it to share data
    // between threads is a data race, whatever intuition Java or C# volatile gave
    // you (in C++ it does not have those semantics). We do not run that version.

    // FIX: std::atomic provides both the atomicity and the memory ordering that
    // thread communication actually needs.
    std::atomic<long> counter{0};
    const int threads = 4, perThread = 100000;
    std::vector<std::thread> pool;
    for (int i = 0; i < threads; ++i)
        pool.emplace_back([&] {
            for (int k = 0; k < perThread; ++k) ++counter;   // atomic ++
        });
    for (auto& t : pool) t.join();

    std::cout << "atomic counter = " << counter.load() << '\n';   // 400000, exact
    // Rule: volatile is for hardware/signal-visible memory, atomic is for threads.
    // They are unrelated, and neither substitutes for the other.
    return 0;
}
