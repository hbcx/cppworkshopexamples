#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// Anti-pattern: expecting a memory_order to make a plain data race safe.
//
// A memory_order orders the VISIBILITY of accesses that are already correctly
// synchronized. It does not, by itself, turn a data race into defined behaviour. If two
// threads write the same plain (non-atomic) variable with no happens-before edge
// between them, adding release/acquire on some OTHER atomic changes nothing -- the
// racing writes are still undefined behaviour:
//
//   int shared = 0;
//   thread A: shared = 1;                   // plain write
//   thread B: shared = 2;                   // concurrent plain write -> data race
//   flag.store(x, std::memory_order_release);   // ordering an unrelated atomic does not help
//   // We do not run that.
//
// A memory order only carries ordinary data that sits on the correct side of a
// synchronizes-with edge (written before a release, read after the paired acquire) or
// that is otherwise guarded so one thread touches it at a time. To make concurrent
// UPDATES to a value safe, the value itself must be atomic (or locked). Below the shared
// value is atomic, so the concurrent updates are defined and every one is counted.
int main() {
    std::atomic<long> shared{0};        // atomic: the updates themselves are safe

    std::vector<std::jthread> workers;
    for (int t = 0; t < 8; ++t) {
        workers.emplace_back([&] {
            for (int k = 0; k < 1000; ++k)
                shared.fetch_add(1, std::memory_order_relaxed);
        });
    }
    workers.clear();                    // join all

    std::cout << "the shared value itself is atomic, total = " << shared.load() << '\n';   // 8000
    return 0;
}
