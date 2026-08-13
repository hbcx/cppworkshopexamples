#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

int main() {
    // A vector kept sorted DESCENDING, so its ordering is std::greater. Every
    // binary search on it must use that SAME ordering.
    std::vector<int> v{9, 7, 5, 3, 1};

    // Find where 6 belongs in the descending order: lower_bound with std::greater
    // returns the first position not ordered-before 6 -- the sorted insertion point.
    auto pos = std::lower_bound(v.begin(), v.end(), 6, std::greater<int>());
    v.insert(pos, 6);

    std::cout << "after inserting 6 (kept descending): ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
