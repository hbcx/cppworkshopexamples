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

    // std::make_heap rearranges the range in place into a MAX-HEAP: a binary heap
    // where every parent is >= its children. The single guarantee you get is that
    // the largest element is at the FRONT -- the rest is only partially ordered.
    std::make_heap(v.begin(), v.end());

    print("heap layout:", v);            // one valid heap order, NOT sorted
    std::cout << "largest (front): " << v.front() << '\n';
    std::cout << "is a heap? " << std::is_heap(v.begin(), v.end()) << '\n';

    // std::is_heap_until returns the end of the longest prefix that is a heap --
    // for a full heap that is end(), so the whole range qualifies.
    auto good = std::is_heap_until(v.begin(), v.end());
    std::cout << "heap prefix length: " << (good - v.begin()) << " of " << v.size()
              << '\n';
    return 0;
}
