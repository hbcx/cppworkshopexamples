#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{7, 2, 9, 4, 1, 8, 5};   // 7 elements

    // nth_element places the element that WOULD sit at position n in a fully sorted
    // range into that position, and partitions the rest around it: everything before
    // is <= it, everything after is >= it. It does this in O(n) on average, without
    // sorting either side. Here n = 3 (the middle of 7) gives the median.
    std::size_t n = v.size() / 2;   // index 3
    std::nth_element(v.begin(), v.begin() + n, v.end());

    std::cout << "median (4th smallest) = " << v[n] << '\n';
    return 0;
}
