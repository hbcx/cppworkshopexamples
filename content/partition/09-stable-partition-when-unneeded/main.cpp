#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{5, 2, 9, 1, 6, 3, 8, 4};
    auto isBig = [](int x) { return x >= 5; };

    // The task: just SEPARATE the big values from the small ones to count or
    // process each group -- the order within a group does not matter.

    // BAD: reaching for stable_partition anyway. It preserves an intra-group order
    // you do not care about, and pays for it with a temporary buffer (O(n) extra
    // memory) and more work than the in-place version.
    std::vector<int> bad = v;
    std::stable_partition(bad.begin(), bad.end(), isBig);

    // GOOD: plain partition splits in place with no allocation. When order within
    // the groups is irrelevant, this is all you need.
    std::vector<int> good = v;
    auto point = std::partition(good.begin(), good.end(), isBig);

    std::cout << "big count: " << std::distance(good.begin(), point)
              << ", small count: " << std::distance(point, good.end()) << '\n';
    return 0;
}
