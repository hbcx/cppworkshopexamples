#include <iostream>
#include <set>

int main() {
    std::set<int> s{10, 20, 30, 40, 50, 60};

    // lower_bound(x): first element >= x.  upper_bound(x): first element > x.
    // Both are O(log n) on a set, and are exactly what an unordered_set cannot do.
    auto lo = s.lower_bound(25);   // -> 30
    auto hi = s.upper_bound(50);   // -> 60
    std::cout << "lower_bound(25) = " << *lo << '\n';
    std::cout << "upper_bound(50) = " << *hi << '\n';

    // A range query: every element in [25, 50].
    std::cout << "elements in [25, 50]:";
    for (auto it = s.lower_bound(25); it != s.upper_bound(50); ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << '\n';

    // The smallest and largest are just begin() and rbegin().
    std::cout << "min = " << *s.begin() << ", max = " << *s.rbegin() << '\n';
    return 0;
}
