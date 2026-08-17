#include <iostream>
#include <semaphore>
#include <thread>
#include <vector>

// acquire() and release() must be balanced on EVERY path -- an early return or a
// thrown exception between them leaks a permit forever. Wrap them the way you wrap
// a mutex with std::lock_guard: an RAII guard that acquires in its constructor and
// releases in its destructor, so the permit comes back no matter how the scope
// exits. The standard gives no such guard for semaphores, so a small one is worth
// keeping.
template <std::ptrdiff_t Max>
class SemaphoreGuard {
    std::counting_semaphore<Max>& sem_;
public:
    explicit SemaphoreGuard(std::counting_semaphore<Max>& s) : sem_(s) { sem_.acquire(); }
    ~SemaphoreGuard() { sem_.release(); }
    SemaphoreGuard(const SemaphoreGuard&) = delete;
    SemaphoreGuard& operator=(const SemaphoreGuard&) = delete;
};

int main() {
    const int LIMIT = 2;
    std::counting_semaphore<2> slots{LIMIT};

    std::vector<std::jthread> workers;
    for (int i = 0; i < 6; ++i) {
        workers.emplace_back([&, i] {
            SemaphoreGuard<2> guard{slots};   // acquire here; release on any exit
            if (i % 2 == 0) return;           // early exit -- permit still released
            // ... normal work ...
        });
    }
    workers.clear();                          // join all

    // Nothing leaked: both permits are free again, so two acquires succeed.
    bool a = slots.try_acquire();
    bool b = slots.try_acquire();
    std::cout << "all permits returned via RAII = " << std::boolalpha << (a && b) << '\n';  // true
    return 0;
}
