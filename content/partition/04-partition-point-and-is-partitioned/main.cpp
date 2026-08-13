#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    // A range already partitioned: all evens, then all odds.
    std::vector<int> v{2, 4, 6, 8, 1, 3, 5};
    auto isEven = [](int x) { return x % 2 == 0; };

    // is_partitioned checks the property: do all satisfying elements come before
    // all non-satisfying ones?
    std::cout << "is_partitioned: " << std::boolalpha
              << std::is_partitioned(v.begin(), v.end(), isEven) << '\n';

    // partition_point binary-searches a PARTITIONED range for the boundary -- the
    // first element that does not satisfy the predicate -- in O(log n), without
    // scanning every element.
    auto point = std::partition_point(v.begin(), v.end(), isEven);
    std::cout << "boundary at index " << std::distance(v.begin(), point)
              << ", first non-even = " << *point << '\n';
    return 0;
}
