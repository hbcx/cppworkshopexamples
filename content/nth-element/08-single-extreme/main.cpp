#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{50, 12, 88, 7, 33, 91, 4, 60};

    // BAD: using nth_element (or partial_sort) just to get the single smallest
    // element. Both REORDER the range to do it -- more work and a side effect --
    // when a single extreme needs neither.
    std::vector<int> bad = v;
    std::nth_element(bad.begin(), bad.begin(), bad.end());   // smallest to the front
    std::cout << "smallest via nth_element: " << bad.front() << '\n';

    // GOOD: min_element scans once in O(n), returns an iterator, and does NOT modify
    // the range. Use it (or max_element) for a single min or max.
    auto it = std::min_element(v.begin(), v.end());
    std::cout << "smallest via min_element: " << *it
              << " (range untouched: " << std::boolalpha
              << (v.front() == 50) << ")\n";
    return 0;
}
