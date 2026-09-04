#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> v{5, 10, 15, 20, 25, 30};

    // std::distance counts the steps from first to last -- the number of elements
    // in the half-open range [first, last).
    std::cout << "size via distance = " << std::distance(v.begin(), v.end()) << "\n";  // 6

    // A common use: the INDEX of an element an algorithm found, from begin to it.
    auto it = std::find(v.begin(), v.end(), 20);
    std::cout << "index of 20 = " << std::distance(v.begin(), it) << "\n";   // 3

    // The length of a sub-range between two iterators.
    auto from = std::find(v.begin(), v.end(), 10);
    auto to   = std::find(v.begin(), v.end(), 25);
    std::cout << "elements in [10, 25) = " << std::distance(from, to) << "\n";   // 3

    // distance requires first to be reachable from last by ++ (for non-random-
    // access iterators); passing them in the wrong order is undefined there.
    return 0;
}
