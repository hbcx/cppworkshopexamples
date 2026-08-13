#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{2, 4, 6, 8, 10, 12};   // sorted
    int target = 8;

    // BAD: binary_search tells you only WHETHER the value is present, not where.
    // Following it with std::find to get the position throws away the O(log n) win
    // -- the find is a linear O(n) scan, so the pair is slower than one lookup.
    if (std::binary_search(v.begin(), v.end(), target)) {
        auto it = std::find(v.begin(), v.end(), target);   // O(n), redundant
        std::cout << "binary_search + find index: "
                  << std::distance(v.begin(), it) << '\n';
    }

    // GOOD: lower_bound gives the POSITION in one O(log n) search; confirm presence
    // by checking it is not end() and really equals the target.
    auto it = std::lower_bound(v.begin(), v.end(), target);
    if (it != v.end() && *it == target)
        std::cout << "lower_bound index:          "
                  << std::distance(v.begin(), it) << '\n';
    return 0;
}
