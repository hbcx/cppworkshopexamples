#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7};

    // rotate(first, middle, last) rearranges the range so that `middle` becomes
    // the new FIRST element; the front part [first, middle) wraps around to the
    // back. It is a left rotation by (middle - first) positions.
    auto old_first = std::rotate(v.begin(), v.begin() + 3, v.end());

    // The element that was at index 3 (value 4) is now at the front, and 1 2 3
    // wrapped to the end.
    std::cout << "after rotate: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // rotate returns an iterator to where the OLD first element (1) ended up.
    std::cout << "old first (1) now at index "
              << std::distance(v.begin(), old_first) << '\n';
    return 0;
}
