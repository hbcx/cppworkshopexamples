#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// Anti-pattern: building a read-modify-write out of a separate load() and store().
//
// Each atomic_ref operation is individually atomic, but two of them are not atomic
// together. Doing "increment" as load() then store() opens a window between the two
// where another thread can slip in, so updates are lost -- exactly the bug atomicity
// was meant to prevent:
//
//   int v = ref.load();
//   ref.store(v + 1);        // another thread may have stored between load and store
//   // Lost updates: the final count comes out too low.
//   // We do not run that broken version.
//
// The fix is a single read-modify-write operation -- fetch_add for +, or a
// compare_exchange loop for anything the fetch_ family does not offer -- so the read
// and the write are one indivisible step. Below fetch_add gives the exact count.
int main() {
    int counter = 0;

    std::vector<std::jthread> workers;
    for (int t = 0; t < 8; ++t) {
        workers.emplace_back([&] {
            std::atomic_ref<int> ref{counter};
            for (int k = 0; k < 1000; ++k)
                ref.fetch_add(1, std::memory_order_relaxed);   // one atomic RMW, not load+store
        });
    }
    workers.clear();                    // join all

    std::cout << "fetch_add (not load+store), counter = " << counter << '\n';   // 8000
    return 0;
}
