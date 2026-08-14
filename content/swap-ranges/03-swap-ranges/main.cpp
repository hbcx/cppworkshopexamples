#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> a{1, 2, 3, 4};
    std::vector<int> b{10, 20, 30, 40};

    // swap_ranges swaps the elements of two equal-length ranges, pairwise. After
    // the call a and b have exchanged contents.
    std::swap_ranges(a.begin(), a.end(), b.begin());
    std::cout << "a: ";
    for (int x : a) std::cout << x << ' ';
    std::cout << "\nb: ";
    for (int x : b) std::cout << x << ' ';
    std::cout << '\n';

    // It also swaps a SUB-RANGE of one container with part of another: swap the
    // first two elements of c with the first two of a.
    std::vector<int> c{7, 7, 7, 7};
    std::swap_ranges(c.begin(), c.begin() + 2, a.begin());
    std::cout << "c: ";
    for (int x : c) std::cout << x << ' ';
    std::cout << " | a: ";
    for (int x : a) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
