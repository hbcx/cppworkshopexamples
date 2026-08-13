#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{5, 1, 9, 3, 7};   // NOT sorted
    std::cout << std::boolalpha;

    // BAD: binary_search (like every binary search) requires a sorted range. On
    // unsorted data its halving logic follows a wrong path -- the behavior is
    // undefined and it can report a present value as missing. 9 IS in the vector,
    // yet the search is not entitled to find it. Shown only as a comment:
    //   std::binary_search(v.begin(), v.end(), 9);   // precondition violated: UB

    // GOOD, option 1: sort first, then binary_search is correct and O(log n).
    std::vector<int> sorted = v;
    std::sort(sorted.begin(), sorted.end());
    std::cout << "binary_search(9) after sort: "
              << std::binary_search(sorted.begin(), sorted.end(), 9) << '\n';

    // GOOD, option 2: for a one-off lookup on an unsorted range, find is O(n) but
    // needs no sort -- cheaper than sorting when you search once.
    std::cout << "find(9) on unsorted:         "
              << (std::find(v.begin(), v.end(), 9) != v.end()) << '\n';
    return 0;
}
