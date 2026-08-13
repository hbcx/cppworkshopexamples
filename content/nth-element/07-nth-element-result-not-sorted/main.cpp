#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{7, 2, 9, 4, 1, 8, 5, 3};
    const std::size_t k = 4;

    // nth_element places the kth element and PARTITIONS the rest around it: the k
    // elements before position k are the k smallest, but in UNSPECIFIED order -- they
    // are NOT sorted. Reading v[0..k) expecting ascending order is a bug.
    std::nth_element(v.begin(), v.begin() + k, v.end());

    // The value AT position k is correct (the kth smallest), and the SET before it is
    // the k smallest -- but their order is unspecified, so they are not printed here.
    std::cout << "kth smallest (position " << k << ") = " << v[k] << '\n';

    // GOOD: if you need the bottom k IN ORDER, use partial_sort, which sorts them.
    std::vector<int> w{7, 2, 9, 4, 1, 8, 5, 3};
    std::partial_sort(w.begin(), w.begin() + k, w.end());
    std::cout << "bottom " << k << " in order: ";
    for (std::size_t i = 0; i < k; ++i) std::cout << w[i] << ' ';
    std::cout << '\n';
    return 0;
}
