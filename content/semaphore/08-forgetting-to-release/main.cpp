#include <iostream>
#include <semaphore>
#include <thread>
#include <vector>

// Anti-pattern: an acquire() with no matching release() on some path.
//
// A counting_semaphore only hands a permit back when you release it. If a worker
// takes an early exit -- an error return, a thrown exception, a branch that skips
// the release -- that permit is gone for good. It does not just block one object:
// it permanently shrinks the pool, and once every permit has leaked the next
// acquire() blocks FOREVER and the whole pool deadlocks:
//
//   slots.acquire();
//   if (failed) return;        // <-- leaked: this permit is never released
//   slots.release();
//   ... enough leaks and acquire() hangs for good ...
//   // We do not run that deadlocking version.
//
// The fix is RAII: a guard that releases in its destructor, so the permit returns
// on EVERY exit -- normal, early return, or exception -- just like std::lock_guard
// does for a mutex.
template <std::ptrdiff_t Max>
struct SemaphoreRelease {
    std::counting_semaphore<Max>& sem;
    ~SemaphoreRelease() { sem.release(); }   // fires however the scope ends
};

int main() {
    const int LIMIT = 2;
    std::counting_semaphore<2> slots{LIMIT};

    std::vector<std::jthread> workers;
    for (int i = 0; i < 6; ++i) {
        workers.emplace_back([&, i] {
            slots.acquire();
            SemaphoreRelease<2> releaser{slots};   // guarantees the matching release
            if (i % 2 == 0) return;                // "early failure" -- permit still freed
            // ... normal work ...
        });
    }
    workers.clear();                               // join all

    // No permit leaked: both are free again.
    bool a = slots.try_acquire();
    bool b = slots.try_acquire();
    std::cout << "no permit leaked, both free again = " << std::boolalpha << (a && b) << '\n';  // true
    return 0;
}
