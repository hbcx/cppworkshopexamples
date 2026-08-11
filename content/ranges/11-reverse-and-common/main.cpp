#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    // views::reverse walks a range back to front, lazily, with no copy.
    std::cout << "reversed:";
    for (int x : v | std::views::reverse) std::cout << " " << x;   // 5 4 3 2 1
    std::cout << "\n";

    // Some views are NOT common ranges: begin() and end() have DIFFERENT types
    // (an iterator and a sentinel), which a legacy (first, last) API of the SAME
    // type cannot take. views::common bridges the gap.
    auto evens = v | std::views::filter([](int x){ return x % 2 == 0; });
    auto common = evens | std::views::common;                        // begin/end share a type
    int total = std::accumulate(common.begin(), common.end(), 0);    // pre-ranges algorithm
    std::cout << "sum of evens via a legacy algorithm: " << total << "\n";   // 6
    return 0;
}
