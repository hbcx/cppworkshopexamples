#include <iostream>
#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>

// Anti-pattern: using a semaphore as a mutex.
//
// A binary_semaphore looks like a lock -- acquire before, release after -- but it
// is not one, and the differences bite:
//   * No OWNER. A std::mutex may only be unlocked by the thread that locked it; a
//     semaphore lets ANY thread release, so a stray release elsewhere silently
//     "unlocks" your critical section and lets a second thread in.
//   * Unbalanced release is UNDEFINED BEHAVIOUR, not an error. release() when the
//     permit is already at its maximum (1 for a binary_semaphore) is UB -- a double
//     or recursive "unlock" a mutex would simply define. We do NOT run that:
//         std::binary_semaphore lock{1};
//         lock.release();   // count 1 -> 2 exceeds the max: UB
//   * No RAII. There is no lock_guard for a semaphore, so an early return between
//     acquire() and release() leaks the permit and deadlocks every later acquirer.
//
// The fix is to use the right tool: std::mutex + std::lock_guard for mutual
// exclusion (owner, RAII, defined behaviour on misuse). Keep semaphores for
// SIGNALING and for COUNTING permits. Below is the mutex version, run to show the
// guarded counter comes out exact and race-free.
int main() {
    std::mutex m;
    long counter = 0;

    std::vector<std::jthread> workers;
    for (int i = 0; i < 8; ++i) {
        workers.emplace_back([&] {
            for (int k = 0; k < 1000; ++k) {
                std::lock_guard<std::mutex> guard{m};   // owner + RAII release
                ++counter;
            }
        });
    }
    workers.clear();                                    // join all

    std::cout << "8 threads x 1000, mutex-guarded counter = " << counter << '\n';  // 8000
    return 0;
}
