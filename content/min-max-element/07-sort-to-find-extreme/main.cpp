#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> prices{42, 17, 88, 5, 63, 29};

    // BAD: sorting the whole vector just to read the smallest and largest. sort is
    // O(n log n) and reorders (destroys) the original order, all to look at two
    // ends. On a copy here to keep the source, which also costs an allocation.
    std::vector<int> sorted = prices;
    std::sort(sorted.begin(), sorted.end());
    std::cout << "via sort:   min " << sorted.front()
              << ", max " << sorted.back() << '\n';

    // GOOD: minmax_element scans once, O(n), and leaves the data untouched. Use
    // min_element or max_element when you only need one end.
    auto mm = std::minmax_element(prices.begin(), prices.end());
    std::cout << "via minmax: min " << *mm.first << ", max " << *mm.second << '\n';
    return 0;
}
