#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// The control block's reference count is ATOMIC: copying and destroying
// shared_ptrs that point at the same object from several threads is safe and
// never loses a count update. What is NOT protected is the pointee -- two
// threads writing the managed object still need their own synchronisation.
struct Counter {
    int value = 0;
};

int main() {
    auto shared = std::make_shared<Counter>();

    // Each thread repeatedly makes and destroys its own copies of `shared`. Only
    // the reference count is touched concurrently -- the thread-safe part -- and
    // the object is only READ, so this is race-free (and clean under
    // ThreadSanitizer where the toolchain provides it; CI runs it on Linux).
    const int threadCount = 4;
    const int iterations = 10000;
    std::vector<std::thread> pool;
    for (int t = 0; t < threadCount; ++t) {
        pool.emplace_back([shared]() {
            for (int i = 0; i < iterations; ++i) {
                std::shared_ptr<Counter> local = shared;   // atomic ++count
                (void) local->value;                       // read only: no race
            }                                              // atomic --count
        });
    }
    for (std::thread& th : pool) {
        th.join();
    }

    // Every copy has been destroyed, so the count is back to exactly 1. A
    // non-atomic count would have lost updates under the contention above and
    // this would not reliably hold.
    std::cout << "final use_count = " << shared.use_count() << '\n';   // 1

    // NOTE: writing shared->value from several threads at once would be a data
    // race on the Counter itself -- the atomic reference count does NOT protect
    // it. That write is deliberately omitted; it would be undefined behaviour and
    // ThreadSanitizer would (correctly) flag it.
    return 0;
}
