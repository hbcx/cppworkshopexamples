#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

static void print(const char* label, const std::vector<int>& v) {
    std::cout << label;
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    std::cout << std::boolalpha;
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};
    std::make_heap(v.begin(), v.end());

    // ANTI-PATTERN: assuming a heap is sorted. A max-heap only guarantees the
    // FRONT is the maximum; the rest is partially ordered. Iterating the range
    // expecting descending (or ascending) order gives the wrong sequence.
    print("heap order:  ", v);           // e.g. 9 6 4 3 1 5 2 1 -- NOT sorted
    std::cout << "is_sorted (descending)? "
              << std::is_sorted(v.begin(), v.end(), std::greater<int>()) << '\n';

    // FIX (want a sorted array): sort_heap turns the heap into a sorted range.
    // Note this DESTROYS the heap -- it is no longer usable as one afterwards.
    std::sort_heap(v.begin(), v.end());
    print("after sort_heap:", v);

    // FIX (want elements largest-first from a live heap): pop_heap + pop_back in
    // a loop, which keeps the heap valid at every step.
    std::vector<int> h{3, 1, 4, 1, 5, 9, 2, 6};
    std::make_heap(h.begin(), h.end());
    std::cout << "drained in order:";
    while (!h.empty()) {
        std::pop_heap(h.begin(), h.end());
        std::cout << ' ' << h.back();
        h.pop_back();
    }
    std::cout << '\n';
    return 0;
}
