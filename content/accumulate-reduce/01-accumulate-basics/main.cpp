#include <numeric>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

int main() {
    std::vector<int> nums{3, 1, 4, 1, 5, 9, 2, 6};

    // accumulate is a LEFT FOLD: start from an initial value and combine it with
    // each element in turn. With no operation it uses + -- a sum.
    int sum = std::accumulate(nums.begin(), nums.end(), 0);
    std::cout << "sum:     " << sum << '\n';   // 31

    // Pass a binary operation for a different fold. std::multiplies gives a product;
    // start from 1, the identity for multiplication.
    int product = std::accumulate(nums.begin(), nums.end(), 1, std::multiplies<int>{});
    std::cout << "product: " << product << '\n';

    // Any two-argument rule works. Here a running maximum via a lambda -- the first
    // argument is the ACCUMULATOR, the second is the element.
    int maxVal = std::accumulate(nums.begin(), nums.end(), nums[0],
        [](int acc, int x){ return std::max(acc, x); });
    std::cout << "max:     " << maxVal << '\n';   // 9
    return 0;
}
