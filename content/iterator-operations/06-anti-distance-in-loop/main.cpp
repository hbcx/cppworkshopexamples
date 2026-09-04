#include <cstddef>
#include <iostream>
#include <iterator>

// A minimal forward iterator over an int array that COUNTS every ++ in a global,
// so we can measure how much work the iterator operations really do.
long g_steps = 0;

struct CountingIter {
    const int* p;
    using iterator_category = std::forward_iterator_tag;
    using value_type        = int;
    using difference_type   = std::ptrdiff_t;
    using pointer           = const int*;
    using reference         = const int&;

    reference operator*() const { return *p; }
    CountingIter& operator++() { ++p; ++g_steps; return *this; }
    CountingIter operator++(int) { CountingIter t = *this; ++(*this); return t; }
    bool operator==(const CountingIter& o) const { return p == o.p; }
    bool operator!=(const CountingIter& o) const { return p != o.p; }
};

int main() {
    int data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    CountingIter begin{data}, end{data + 8};

    // ANTI-PATTERN: recomputing a position with std::distance inside a loop over a
    // NON-random-access range. Each distance call walks from begin, so an O(n)
    // loop does O(n^2) total steps.
    g_steps = 0;
    for (auto it = begin; it != end; ++it) {
        volatile auto idx = std::distance(begin, it);   // O(n) EACH iteration
        (void)idx;
    }
    std::cout << "distance-in-loop steps = " << g_steps << "\n";   // 36 = 8 + 8*7/2

    // FIX: keep the index as you iterate -- one increment per element, O(n) total.
    g_steps = 0;
    long idx = 0;
    for (auto it = begin; it != end; ++it, ++idx) {
        volatile long keep = idx;
        (void)keep;
    }
    std::cout << "running-index steps    = " << g_steps << "\n";   // 8
    return 0;
}
