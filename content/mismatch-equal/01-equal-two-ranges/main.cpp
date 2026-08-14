#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::cout << std::boolalpha;

    std::vector<int> a{1, 2, 3, 4, 5};
    std::vector<int> b{1, 2, 3, 4, 5};
    std::vector<int> c{1, 2, 9, 4, 5};

    // std::equal walks two ranges in step and returns true only if every pair of
    // elements is equal. The three-argument form takes the FIRST range as a pair
    // and only the start of the second -- it assumes the second is at least as
    // long as the first (see the anti-pattern for why that bites).
    std::cout << "a equals b? " << std::equal(a.begin(), a.end(), b.begin()) << '\n';
    std::cout << "a equals c? " << std::equal(a.begin(), a.end(), c.begin()) << '\n';

    // The four-argument form (C++14) takes BOTH ranges in full, so it also checks
    // the lengths match -- the safe default when the ranges can differ in size.
    std::vector<int> shorter{1, 2, 3};
    std::cout << "a equals shorter? "
              << std::equal(a.begin(), a.end(), shorter.begin(), shorter.end())
              << '\n';

    // For two containers of the SAME type, a == b does the same thing more
    // simply; std::equal earns its place across different container types or
    // with a custom predicate.
    return 0;
}
