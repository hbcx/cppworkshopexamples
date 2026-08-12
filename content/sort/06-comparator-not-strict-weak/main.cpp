#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> nums{5, 2, 8, 2, 9, 1, 5};

    // A comparator for sort must be a STRICT weak ordering. The key rule: comp(x, x)
    // must be FALSE -- nothing is less than itself. Using <= breaks that, and the
    // result is UNDEFINED BEHAVIOR, not merely a wrong order.
    //
    // BAD (shown in a comment -- running it is UB; libstdc++ in debug mode can read
    // out of bounds and crash):
    //
    //   std::sort(nums.begin(), nums.end(), [](int a, int b){ return a <= b; });
    //
    // With <=, equal elements each compare "less" than the other, so sort's internal
    // bounds logic can walk PAST the end of the range -- memory corruption, not just
    // a bad order.

    // GOOD: use < (strict). Equal elements correctly compare as neither below the
    // other, and equivalent order is defined.
    std::sort(nums.begin(), nums.end(), [](int a, int b){ return a < b; });
    std::cout << "sorted with a strict comparator: ";
    for (int x : nums) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
