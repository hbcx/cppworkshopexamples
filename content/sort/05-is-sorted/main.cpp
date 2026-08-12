#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> a{1, 2, 3, 4, 5};
    std::vector<int> b{1, 2, 5, 3, 4};

    std::cout << std::boolalpha;
    // is_sorted: is the range already in order? A cheap O(n) check to skip a
    // needless sort, or to confirm a precondition (like before a binary_search).
    std::cout << "a sorted? " << std::is_sorted(a.begin(), a.end()) << '\n';   // true
    std::cout << "b sorted? " << std::is_sorted(b.begin(), b.end()) << '\n';   // false

    // is_sorted_until returns the first element that breaks the order -- the point
    // where sortedness stops.
    auto brk = std::is_sorted_until(b.begin(), b.end());
    std::cout << "b is sorted up to index " << (brk - b.begin())
              << " (first out-of-order value: " << *brk << ")\n";   // index 3, value 3
    return 0;
}
