#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

int main() {
    std::vector<int> nums{1, 2, 3, 4, 5};

    // transform applies a function to each element and writes the result to an
    // output iterator. To a NEW container, a back_inserter appends each result, so
    // the output grows as needed.
    std::vector<int> squares;
    std::transform(nums.begin(), nums.end(), std::back_inserter(squares),
        [](int x){ return x * x; });
    std::cout << "squares:  ";
    for (int x : squares) std::cout << x << ' ';
    std::cout << '\n';

    // IN PLACE: the output can be the input range itself -- results are written
    // back over the source, element for element.
    std::transform(nums.begin(), nums.end(), nums.begin(), [](int x){ return x + 100; });
    std::cout << "in place: ";
    for (int x : nums) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
