#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> a{1, 2, 3, 5, 8};   // sorted
    std::vector<int> b{2, 3, 4, 8, 9};   // sorted

    // set_union: every element in EITHER range, with each value appearing once
    // (for unique inputs). The output is sorted.
    std::vector<int> u;
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(u));
    std::cout << "union:        ";
    for (int x : u) std::cout << x << ' ';
    std::cout << '\n';

    // set_intersection: only the elements present in BOTH ranges.
    std::vector<int> i;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                          std::back_inserter(i));
    std::cout << "intersection: ";
    for (int x : i) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
