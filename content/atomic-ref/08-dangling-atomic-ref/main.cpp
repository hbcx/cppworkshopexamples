#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// Anti-pattern: an atomic_ref that outlives, or is invalidated by moving, the object
// it references.
//
// atomic_ref does NOT own its object -- it is a reference. If the object goes away
// while the atomic_ref is still used, or its address changes, the ref dangles and
// you have undefined behaviour. A classic trap is binding an atomic_ref to a
// std::vector element and then GROWING the vector: a reallocation moves the elements
// to new storage and the ref still points at the freed old buffer:
//
//   std::vector<int> v{0};
//   std::atomic_ref<int> ref{v[0]};
//   v.push_back(1);              // may reallocate -> ref now points at freed memory
//   ref.fetch_add(1);           // UB: writes into the old buffer
//   // We do not run that.
//
// The fix is to keep the storage stable for the ref's whole lifetime: size the
// container up front (a fixed size or reserve) so it never reallocates, and keep the
// referenced object alive at least as long as any atomic_ref to it.
int main() {
    const int N = 4;
    std::vector<int> counters(N, 0);    // fixed size: the buffer never moves below

    std::vector<std::jthread> workers;
    for (int t = 0; t < N; ++t) {
        workers.emplace_back([&, t] {
            // Safe: counters is never resized, so &counters[t] stays valid throughout.
            std::atomic_ref<int> ref{counters[t]};
            for (int k = 0; k < 1000; ++k)
                ref.fetch_add(1, std::memory_order_relaxed);
        });
    }
    workers.clear();                    // join all

    long total = 0;
    for (int c : counters) total += c;
    std::cout << "stable storage, no dangling ref, total = " << total << '\n';   // 4000
    return 0;
}
