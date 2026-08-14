#include <algorithm>
#include <iostream>
#include <utility>

int main() {
    int a = 7, b = 3;

    // std::minmax returns BOTH the smaller and the larger in one call, as a
    // std::pair{min, max}. Handy when you need both and do not want to write
    // std::min and std::max separately.
    std::pair<const int&, const int&> both = std::minmax(a, b);
    std::cout << "minmax(7, 3): min=" << both.first
              << " max=" << both.second << '\n';

    // Like min/max it also takes an initializer_list, giving the extremes of a
    // whole set at once -- the smallest and largest of these five in one shot.
    auto span = std::minmax({4, 8, 1, 6, 2});
    std::cout << "minmax of {4,8,1,6,2}: min=" << span.first
              << " max=" << span.second << '\n';
    std::cout << "range width = " << (span.second - span.first) << '\n';

    // Structured bindings (C++17) read even better: auto [lo, hi] = minmax(...).
    // Note: minmax guarantees min is the FIRST of equal elements and max the
    // LAST, which matters only when elements compare equal.
    return 0;
}
