#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> nums{3, 7, 3, 9, 3, 7, 1};

    // count returns how many elements EQUAL a value; count_if how many satisfy a
    // predicate. Both return a signed integer (the iterator difference type).
    auto threes = std::count(nums.begin(), nums.end(), 3);
    std::cout << "count of 3: " << threes << '\n';   // 3

    auto over5 = std::count_if(nums.begin(), nums.end(), [](int x){ return x > 5; });
    std::cout << "count over 5: " << over5 << '\n';   // 3 (7, 9, 7)

    // A value that is absent simply counts as 0 -- nothing to check first.
    std::cout << "count of 100: " << std::count(nums.begin(), nums.end(), 100) << '\n';  // 0
    return 0;
}
