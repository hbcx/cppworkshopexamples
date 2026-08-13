#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};

    // partition rearranges the range so that every element satisfying the
    // predicate comes BEFORE every element that does not. It returns an iterator
    // to the partition point -- the first element that does NOT satisfy it.
    auto point = std::partition(v.begin(), v.end(),
                                [](int x) { return x % 2 == 0; });

    // Everything in [begin, point) is even; everything in [point, end) is odd.
    std::cout << "evens count: " << std::distance(v.begin(), point) << '\n';
    std::cout << "evens: ";
    for (auto it = v.begin(); it != point; ++it) std::cout << *it << ' ';
    std::cout << "| odds: ";
    for (auto it = point; it != v.end(); ++it) std::cout << *it << ' ';
    std::cout << '\n';
    return 0;
}
