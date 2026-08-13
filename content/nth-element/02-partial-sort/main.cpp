#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{7, 2, 9, 4, 1, 8, 5, 3};

    // partial_sort sorts only the first k elements: after the call, [begin, begin+k)
    // holds the k SMALLEST elements in sorted order, and the rest are in unspecified
    // order. It costs O(n log k) -- cheaper than a full sort when k is small.
    const std::size_t k = 3;
    std::partial_sort(v.begin(), v.begin() + k, v.end());

    std::cout << "3 smallest, in order: ";
    for (std::size_t i = 0; i < k; ++i) std::cout << v[i] << ' ';
    std::cout << '\n';
    return 0;
}
