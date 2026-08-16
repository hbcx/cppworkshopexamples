#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

long expensive(int seed) {           // stand-in for slow, thread-independent work
    long s = 0;
    for (int k = 1; k <= 2000; ++k) s += static_cast<long>(seed) * k;
    return s;
}

int main() {
    std::mutex m;
    long total = 0;

    // ANTI-PATTERN: doing the slow, independent work WHILE holding the lock. Every
    // thread has to wait its turn for the whole computation, so the mutex serializes
    // work that never needed protecting -- the threads effectively run one at a time.
    //
    //   std::lock_guard<std::mutex> g(m);
    //   total += expensive(id);     // expensive() runs INSIDE the critical section
    //
    // It still gives the right answer, which is why the lost parallelism is easy to
    // miss -- it shows up only as poor scaling. Below is the fix; both compute the
    // same total.

    // FIX: run expensive() with NO lock held so the threads compute in parallel,
    // then lock only for the tiny shared update.
    std::vector<std::thread> pool;
    for (int i = 1; i <= 4; ++i)
        pool.emplace_back([&, i] {
            long r = expensive(i);                    // parallel: no lock held
            std::lock_guard<std::mutex> g(m);         // lock only for the +=
            total += r;
        });
    for (auto& t : pool) t.join();

    std::cout << "total = " << total << '\n';   // 20010000

    // Keep critical sections SHORT: hold a lock only around the shared-data access,
    // never around I/O, memory allocation, or computation that touches no shared
    // state. A lock held too long turns a parallel program back into a serial one.
    return 0;
}
