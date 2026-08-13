#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> a{1, 2, 3, 4};
    std::vector<int> b{4, 3, 2, 1};   // same elements, different order
    std::vector<int> c{1, 2, 2, 4};   // different multiset

    // is_permutation tests whether two ranges hold the SAME elements in some order
    // -- the same values with the same multiplicities. It needs NO sorting and
    // does not modify either range, unlike sorting both and comparing.
    std::cout << std::boolalpha;
    std::cout << "b is a permutation of a? "
              << std::is_permutation(a.begin(), a.end(), b.begin()) << '\n';
    std::cout << "c is a permutation of a? "
              << std::is_permutation(a.begin(), a.end(), c.begin()) << '\n';
    return 0;
}
