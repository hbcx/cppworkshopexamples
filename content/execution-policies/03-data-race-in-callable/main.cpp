#include <atomic>
#include <execution>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> data(1000);
    for (int i = 0; i < 1000; ++i) data[i] = i + 1;   // 1..1000

    // ANTI-PATTERN: mutate shared state from a par callable without synchronizing.
    //
    //   int count = 0;
    //   std::for_each(std::execution::par, data.begin(), data.end(),
    //                 [&](int x){ if (x % 2 == 0) ++count; });   // DATA RACE -> UB
    //
    // Under par the callable runs on many threads at once, so the unsynchronized
    // ++count is a data race: undefined behaviour. It may lose increments, tear
    // the value, or crash -- and it can pass on a serial build only to fail once
    // it really parallelizes. We do not run that version.

    // FIX 1: make the shared counter atomic, so concurrent increments are safe.
    std::atomic<int> atomicCount{0};
    std::for_each(std::execution::par, data.begin(), data.end(),
                  [&](int x) { if (x % 2 == 0) ++atomicCount; });
    std::cout << "evens (atomic): " << atomicCount.load() << '\n';

    // FIX 2 (better): avoid shared mutable state entirely. transform_reduce maps
    // each element to 0 or 1 and sums them, so there is nothing to race on -- the
    // parallel-first way to count.
    int reduced = std::transform_reduce(
        std::execution::par, data.begin(), data.end(), 0,
        std::plus<int>{}, [](int x) { return x % 2 == 0 ? 1 : 0; });
    std::cout << "evens (reduce): " << reduced << '\n';
    return 0;
}
