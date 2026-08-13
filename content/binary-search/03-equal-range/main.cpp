#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 4, 4, 4, 6, 8};

    // equal_range returns BOTH bounds at once as a pair {lower, upper} -- the
    // half-open sub-range of all elements equal to the value, in one call (and one
    // O(log n) search rather than a separate lower_bound and upper_bound).
    auto range = std::equal_range(v.begin(), v.end(), 4);

    std::cout << "4 appears " << std::distance(range.first, range.second)
              << " times, at indices "
              << std::distance(v.begin(), range.first) << ".."
              << std::distance(v.begin(), range.second) - 1 << '\n';

    // A missing value gives an EMPTY range (first == second) at the insertion point.
    auto none = std::equal_range(v.begin(), v.end(), 5);
    std::cout << "5 appears " << std::distance(none.first, none.second) << " times\n";
    return 0;
}
