#include <algorithm>
#include <execution>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{5, 3, 1, 4, 2, 8, 6, 7};

    // Since C++17 many algorithms accept an EXECUTION POLICY as their first
    // argument. The policy does not change WHAT the algorithm computes, only HOW
    // it may run. You opt in by passing one of the std::execution:: tags and
    // including <execution>.

    // seq: sequential, like the ordinary overload -- one thread, defined order.
    std::sort(std::execution::seq, v.begin(), v.end());

    // par: the work may be split across multiple THREADS. The result is the same
    // sorted range; only the internal scheduling differs.
    std::sort(std::execution::par, v.begin(), v.end());

    std::cout << "sorted:";
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';

    // par_unseq: threads AND vectorization (SIMD) -- the strongest permission, and
    // the strictest contract: element steps may interleave within a thread, so the
    // callable must not synchronize or depend on order (see the anti-patterns).
    std::for_each(std::execution::par_unseq, v.begin(), v.end(),
                  [](int& x) { x = x * x; });   // squaring is independent per element

    std::cout << "squared:";
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';

    // (C++20 adds std::execution::unseq: vectorization without threads.) Passing a
    // policy is a PROMISE about your callable; the next examples show the rules.
    return 0;
}
