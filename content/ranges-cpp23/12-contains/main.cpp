#include <ranges>
#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{4, 8, 15, 16, 23, 42};

    // ranges::contains: does the range hold this value? A named, readable stand-in
    // for find(...) != end().
    std::cout << std::boolalpha;
    std::cout << "contains 16: " << std::ranges::contains(v, 16) << '\n';
    std::cout << "contains 17: " << std::ranges::contains(v, 17) << '\n';

    // ranges::contains_subrange: does the range contain this CONSECUTIVE run?
    std::vector<int> run{15, 16, 23};
    std::cout << "contains_subrange {15,16,23}: "
              << std::ranges::contains_subrange(v, run) << '\n';
    std::vector<int> gap{15, 23};   // present, but not consecutive in v
    std::cout << "contains_subrange {15,23}: "
              << std::ranges::contains_subrange(v, gap) << '\n';
    return 0;
}
