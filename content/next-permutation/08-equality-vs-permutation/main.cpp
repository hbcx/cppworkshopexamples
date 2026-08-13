#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> expected{1, 2, 3, 4};
    std::vector<int> result{3, 1, 4, 2};   // same elements, order should not matter

    std::cout << std::boolalpha;

    // BAD: using operator== to check the collections "have the same elements".
    // == compares element by element IN ORDER, so it reports these equal-content
    // vectors as different just because the order differs.
    std::cout << "expected == result: " << (expected == result) << '\n';   // false

    // GOOD: is_permutation asks the intended question -- same elements in any order
    // -- without sorting or mutating either range.
    std::cout << "is_permutation:     "
              << std::is_permutation(expected.begin(), expected.end(),
                                     result.begin())
              << '\n';   // true
    return 0;
}
