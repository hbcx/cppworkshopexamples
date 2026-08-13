#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> temps{18, 25, 12, 30, 21, 30, 9};

    // min_element and max_element return ITERATORS, not values -- the position of
    // the smallest and largest element. Dereference for the value.
    auto lo = std::min_element(temps.begin(), temps.end());
    auto hi = std::max_element(temps.begin(), temps.end());
    std::cout << "min value: " << *lo << ", max value: " << *hi << '\n';

    // The iterator also gives the position: distance from begin is the index.
    // With two 30s, max_element reports the FIRST one (index 3).
    std::cout << "min at index " << std::distance(temps.begin(), lo)
              << ", max at index " << std::distance(temps.begin(), hi) << '\n';

    // On an empty range both return end(), so guard before dereferencing.
    std::vector<int> empty;
    if (std::max_element(empty.begin(), empty.end()) == empty.end())
        std::cout << "empty range: no max\n";
    return 0;
}
