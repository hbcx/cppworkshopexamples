#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

int main() {
    std::vector<int> nums{4, 8, 15, 16, 23, 42};

    // find looks for a VALUE; find_if looks for the first element matching a
    // PREDICATE. Both return an ITERATOR -- to the found element, or end() if none.
    auto it = std::find(nums.begin(), nums.end(), 16);
    if (it != nums.end())
        std::cout << "found 16 at index " << std::distance(nums.begin(), it) << '\n';

    auto over20 = std::find_if(nums.begin(), nums.end(), [](int x){ return x > 20; });
    if (over20 != nums.end())
        std::cout << "first over 20 is " << *over20
                  << " at index " << std::distance(nums.begin(), over20) << '\n';

    // A miss returns end(), which does not point at an element -- always check.
    auto missing = std::find(nums.begin(), nums.end(), 99);
    std::cout << "99 present? " << std::boolalpha << (missing != nums.end()) << '\n';
    return 0;
}
