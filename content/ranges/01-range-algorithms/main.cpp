#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> v = {5, 3, 8, 1, 9, 2, 8};

    // Range algorithms take a WHOLE range, not a begin/end pair -- shorter, and you
    // cannot accidentally mix iterators from two different containers.
    std::ranges::sort(v);
    std::cout << "sorted:";
    for (int x : v) std::cout << " " << x;
    std::cout << "\n";

    auto it = std::ranges::find(v, 8);
    std::cout << "found 8 at index " << (it - v.begin()) << "\n";
    std::cout << "count of 8: " << std::ranges::count(v, 8) << "\n";
    std::cout << "min " << *std::ranges::min_element(v)
              << ", max " << *std::ranges::max_element(v) << "\n";
    std::cout << std::boolalpha
              << "all positive? " << std::ranges::all_of(v, [](int x){ return x > 0; }) << "\n";
    return 0;
}
