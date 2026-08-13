#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};

    // stable_partition also moves the satisfying elements to the front, but it
    // PRESERVES the relative order within each group: the evens stay 2 4 6 8 and
    // the odds stay 1 3 5 7 -- unlike plain partition, which may reorder them.
    auto point = std::stable_partition(v.begin(), v.end(),
                                       [](int x) { return x % 2 == 0; });

    std::cout << "evens: ";
    for (auto it = v.begin(); it != point; ++it) std::cout << *it << ' ';
    std::cout << "| odds: ";
    for (auto it = point; it != v.end(); ++it) std::cout << *it << ' ';
    std::cout << '\n';
    return 0;
}
