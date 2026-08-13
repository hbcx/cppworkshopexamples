#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 4, 4, 4, 6, 8};   // sorted, with duplicate 4s

    // lower_bound returns the first element NOT LESS than the value (>= value):
    // the first 4 here. upper_bound returns the first element GREATER than it:
    // the 6 here. Between them lie all the elements equal to the value.
    auto lo = std::lower_bound(v.begin(), v.end(), 4);
    auto hi = std::upper_bound(v.begin(), v.end(), 4);

    std::cout << "lower_bound(4) at index " << std::distance(v.begin(), lo)
              << " (value " << *lo << ")\n";
    std::cout << "upper_bound(4) at index " << std::distance(v.begin(), hi)
              << " (value " << *hi << ")\n";
    std::cout << "count of 4s: " << std::distance(lo, hi) << '\n';

    // lower_bound of a MISSING value is the insertion point that keeps it sorted:
    // 5 belongs where the 6 is (index 5).
    auto ins = std::lower_bound(v.begin(), v.end(), 5);
    std::cout << "insertion point for 5: index "
              << std::distance(v.begin(), ins) << '\n';
    return 0;
}
