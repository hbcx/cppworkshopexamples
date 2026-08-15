#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
    // std::unique_lock is a fuller-featured cousin of lock_guard. Both are RAII (they
    // unlock in their destructor), but unique_lock also tracks whether it currently
    // holds the mutex, so ONE unique_lock object can be locked and unlocked again and
    // again. It can additionally:
    //   * be built with std::defer_lock so it does NOT lock on construction;
    //   * unlock() and lock() repeatedly, reusing the same lock object;
    //   * be moved, to hand a held lock out of a function;
    //   * be passed to a std::condition_variable, which requires a unique_lock.
    // lock_guard can do none of that -- it locks once on construction and only
    // unlocks when destroyed -- which makes it a touch cheaper. Default to lock_guard;
    // reach for unique_lock when you need one of these abilities.
    std::mutex m;
    long shared_total = 0;

    std::vector<std::thread> pool;
    for (int t = 0; t < 4; ++t)
        pool.emplace_back([&] {
            // Create ONE unique_lock, deferred (not locked yet), and REUSE it across
            // many lock/unlock cycles. lock_guard cannot express this at all.
            std::unique_lock<std::mutex> lock(m, std::defer_lock);

            for (int batch = 0; batch < 5; ++batch) {
                long local = 0;
                for (int k = 1; k <= 100; ++k) local += k;   // work with NO lock held

                lock.lock();               // take the mutex just for the update...
                shared_total += local;
                lock.unlock();             // ...and release it -- same 'lock', reused
            }
        });
    for (auto& t : pool) t.join();

    std::cout << "shared_total = " << shared_total << '\n';   // 4 * 5 * 5050 = 101000
    return 0;
}
