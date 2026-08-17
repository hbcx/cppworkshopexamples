#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// std::atomic_ref<T> (C++20) applies atomic operations to an EXISTING plain object
// you name -- it does not hold a value of its own, it references yours. That lets you
// make concurrent updates to an ordinary int (or an array element, or a struct
// field) atomic without changing the object's type to std::atomic. Here many threads
// increment a plain int through atomic_ref, so each ++ is one indivisible fetch_add
// and the total is exact instead of racing.
int main() {
    int counter = 0;                    // a completely ordinary int

    std::vector<std::jthread> workers;
    for (int t = 0; t < 8; ++t) {
        workers.emplace_back([&] {
            std::atomic_ref<int> ref{counter};   // an atomic view of that int
            for (int k = 0; k < 1000; ++k) {
                ref.fetch_add(1, std::memory_order_relaxed);   // indivisible ++
            }
        });
    }
    workers.clear();                    // join every worker (jthread destructor)

    // While the refs existed, every access went through them, so no update was lost.
    // Reading counter plainly is safe now: all workers joined, no atomic_ref is live.
    std::cout << "8 threads x 1000 through atomic_ref, counter = " << counter << '\n';  // 8000
    return 0;
}
