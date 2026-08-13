#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

int main() {
    std::vector<int> nums{10, 20, 30, 40, 50};

    // std::ostream_iterator turns copy into a print: each element is written to the
    // stream, followed by the given separator. A classic one-line way to dump a
    // range without a hand-written loop.
    std::cout << "all:     ";
    std::copy(nums.begin(), nums.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';

    // copy_n copies exactly the first N elements -- a snapshot of the start.
    std::cout << "first 3: ";
    std::copy_n(nums.begin(), 3, std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
    return 0;
}
