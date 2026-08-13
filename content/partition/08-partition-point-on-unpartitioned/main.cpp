#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{2, 5, 4, 1, 6, 3};   // NOT partitioned: evens and odds mixed
    auto isEven = [](int x) { return x % 2 == 0; };

    // partition_point, like any binary search, ASSUMES the range is partitioned:
    // all satisfying elements first, then all the rest. On this mixed range that
    // precondition is false.
    std::cout << "is_partitioned: " << std::boolalpha
              << std::is_partitioned(v.begin(), v.end(), isEven) << '\n';

    // BAD: calling partition_point on an unpartitioned range is undefined behavior
    // -- the binary search follows a boundary that is not there and returns a
    // meaningless answer. Shown only as a comment:
    //   auto bogus = std::partition_point(v.begin(), v.end(), isEven);

    // GOOD: partition first (establishing the precondition), THEN partition_point
    // finds the true boundary. For a range you will not partition, count_if or
    // find_if answer without needing the property.
    std::partition(v.begin(), v.end(), isEven);
    auto point = std::partition_point(v.begin(), v.end(), isEven);
    std::cout << "after partition, evens count: "
              << std::distance(v.begin(), point) << '\n';
    return 0;
}
