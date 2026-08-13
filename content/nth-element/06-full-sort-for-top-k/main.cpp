#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> data{50, 12, 88, 7, 33, 91, 4, 60, 21, 77};
    const std::size_t k = 3;

    // BAD: sorting the WHOLE range to read the k smallest. A full sort is O(n log n)
    // and orders all n elements when only k are wanted.
    std::vector<int> full = data;
    std::sort(full.begin(), full.end());
    std::cout << "via full sort:    ";
    for (std::size_t i = 0; i < k; ++i) std::cout << full[i] << ' ';
    std::cout << '\n';

    // GOOD (need them ordered): partial_sort sorts only the first k -- O(n log k).
    std::vector<int> ps = data;
    std::partial_sort(ps.begin(), ps.begin() + k, ps.end());
    std::cout << "via partial_sort: ";
    for (std::size_t i = 0; i < k; ++i) std::cout << ps[i] << ' ';
    std::cout << '\n';

    // GOOD (only need WHICH k, not ordered): nth_element puts the k smallest before
    // position k in O(n) average -- the cheapest when their order does not matter.
    std::vector<int> ne = data;
    std::nth_element(ne.begin(), ne.begin() + k, ne.end());
    std::sort(ne.begin(), ne.begin() + k);   // sorted here only to print for compare
    std::cout << "via nth_element:  ";
    for (std::size_t i = 0; i < k; ++i) std::cout << ne[i] << ' ';
    std::cout << '\n';
    return 0;
}
