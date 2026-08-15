#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
    // ANTI-PATTERN: a mutex that is LOCAL to each thread (or created per call), so
    // every thread locks a DIFFERENT mutex object. A mutex only excludes threads
    // that lock the SAME instance, so these exclude nobody -- the lock is present
    // but protects nothing, and the shared data still races.
    //
    //   auto work = [&]{
    //       std::mutex local;                 // <-- a fresh mutex per thread!
    //       std::lock_guard<std::mutex> g(local);
    //       ++counter;                        // still a data race across threads
    //   };
    //
    // We do NOT run that: it is a genuine data race (undefined behaviour) that
    // ThreadSanitizer would detect and abort. The mutex must be shared by every
    // thread that touches the data.

    // FIX: ONE mutex, declared once outside the threads and captured by reference,
    // so all threads lock the same object.
    long counter = 0;
    std::mutex shared_m;
    std::vector<std::thread> pool;
    for (int i = 0; i < 4; ++i)
        pool.emplace_back([&] {
            for (int k = 0; k < 100000; ++k) {
                std::lock_guard<std::mutex> g(shared_m);   // the SAME mutex for all
                ++counter;
            }
        });
    for (auto& t : pool) t.join();

    std::cout << "counter = " << counter << '\n';   // 400000

    // The lesson: guarding a piece of data means ONE agreed-upon mutex, reachable by
    // everyone who uses that data. A per-call or per-thread mutex, or locking a copy
    // of the mutex, gives the illusion of safety with none of the protection.
    return 0;
}
