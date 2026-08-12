#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> scores{7, 2, 9, 4, 1, 8, 5, 3, 6};

    // partial_sort: put the smallest N in order at the FRONT and leave the rest in
    // unspecified order. Cheaper than a full sort when you only need the top few.
    std::vector<int> top = scores;
    std::partial_sort(top.begin(), top.begin() + 3, top.end());
    std::cout << "smallest 3 (sorted): " << top[0] << ' ' << top[1] << ' ' << top[2] << '\n';

    // nth_element: place the element that WOULD sit at position n in a full sort,
    // and partition around it -- everything before is <=, everything after is >=.
    // O(n) on average, and it does NOT fully sort. Ideal for a median.
    std::vector<int> v = scores;
    auto mid = v.begin() + v.size() / 2;      // position 4 -> the median of 9 values
    std::nth_element(v.begin(), mid, v.end());
    std::cout << "median: " << *mid << '\n';   // 5
    return 0;
}
