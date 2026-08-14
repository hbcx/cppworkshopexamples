#include <numeric>
#include <vector>
#include <functional>
#include <iostream>

int main() {
    std::vector<int> nums{100, 10, 5, 2};

    // std::reduce does not promise the ORDER in which it combines elements (that is
    // what allows parallel execution), so the operation must be ASSOCIATIVE and
    // COMMUTATIVE. Subtraction is neither.
    //
    // BAD (shown in a comment -- the result is UNSPECIFIED and may differ between
    // builds, standard-library versions, or once a parallel policy is added):
    //
    //   int r = std::reduce(nums.begin(), nums.end(), 0, std::minus<int>{});
    //   // Read left to right it would be 0-100-10-5-2 = -117, but reduce may group
    //   // and reorder the subtractions, giving a different number. Do not rely on it.

    // GOOD: for an ORDER-DEPENDENT fold, use accumulate, which is defined to combine
    // strictly left to right.
    int leftToRight = std::accumulate(nums.begin(), nums.end(), 0, std::minus<int>{});
    std::cout << "accumulate with minus (left to right): " << leftToRight << '\n';
    // 0 - 100 - 10 - 5 - 2 = -117

    // The rule: reduce only with an associative, commutative op (+, *, min, max, and,
    // or); accumulate for anything order-dependent -- subtraction, division, string
    // building, or any op where grouping changes the answer.
    return 0;
}
