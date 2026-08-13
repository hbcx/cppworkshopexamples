#include <numeric>
#include <iterator>
#include <functional>
#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2};

    // partial_sum takes an optional binary op, so it can compute a running MAXIMUM
    // instead of a running sum: out[i] = max of in[0..i].
    std::vector<int> runmax;
    std::partial_sum(v.begin(), v.end(), std::back_inserter(runmax),
                     [](int a, int b) { return std::max(a, b); });
    std::cout << "running max: ";
    for (int x : runmax) std::cout << x << ' ';
    std::cout << '\n';

    // A running PRODUCT with std::multiplies -- the prefix products of 1..5 are
    // the factorials 1, 2, 6, 24, 120.
    std::vector<int> nums{1, 2, 3, 4, 5};
    std::vector<int> runprod;
    std::partial_sum(nums.begin(), nums.end(), std::back_inserter(runprod),
                     std::multiplies<int>());
    std::cout << "running product: ";
    for (int x : runprod) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
