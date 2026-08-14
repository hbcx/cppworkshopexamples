#include <algorithm>
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

    // std::sort_heap takes a range that is ALREADY a heap and turns it into a
    // sorted (ascending) range. It works by repeatedly doing pop_heap, which
    // deposits each successive maximum at the back -- so the biggest ends up last.
    std::make_heap(v.begin(), v.end());
    std::sort_heap(v.begin(), v.end());

    print("after sort_heap:", v);
    std::cout << "sorted? " << std::is_sorted(v.begin(), v.end()) << '\n';

    // make_heap + sort_heap together ARE heapsort: an in-place O(n log n) sort
    // with no extra memory and a guaranteed worst case. In everyday code std::sort
    // is the right default (usually faster in practice); reach for the heap
    // operations when you need the heap itself, not just a sorted result.
    return 0;
}
