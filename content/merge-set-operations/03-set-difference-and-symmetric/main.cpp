#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> a{1, 2, 3, 5, 8};   // sorted
    std::vector<int> b{2, 3, 4, 8, 9};   // sorted

    // set_difference: elements in A but NOT in B -- what A has that B lacks.
    std::vector<int> d;
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                        std::back_inserter(d));
    std::cout << "A - B:                ";
    for (int x : d) std::cout << x << ' ';
    std::cout << '\n';

    // set_symmetric_difference: elements in exactly ONE of the two ranges (in A or
    // B but not both) -- the union minus the intersection.
    std::vector<int> s;
    std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(),
                                  std::back_inserter(s));
    std::cout << "symmetric difference: ";
    for (int x : s) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
