#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>

int main() {
    std::vector<int> nums{5, 2, 8, 1, 9, 3};

    // sort orders a range IN PLACE. With no comparator it uses < -- ascending.
    std::sort(nums.begin(), nums.end());
    std::cout << "ascending:  ";
    for (int x : nums) std::cout << x << ' ';
    std::cout << '\n';

    // Pass a comparator to change the order. std::greater sorts descending; a
    // lambda does the same and can express any rule.
    std::sort(nums.begin(), nums.end(), std::greater<int>{});
    std::cout << "descending: ";
    for (int x : nums) std::cout << x << ' ';
    std::cout << '\n';

    std::sort(nums.begin(), nums.end(), [](int a, int b){ return a < b; });   // ascending again
    std::cout << "ascending:  ";
    for (int x : nums) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
