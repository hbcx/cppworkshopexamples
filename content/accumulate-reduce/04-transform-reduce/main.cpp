#include <numeric>
#include <vector>
#include <functional>
#include <iostream>

int main() {
    std::vector<int> a{1, 2, 3, 4};
    std::vector<int> b{10, 20, 30, 40};

    // transform_reduce (C++17) fuses a map and a reduce in one pass. The two-range
    // form multiplies matched elements and sums the products -- a dot product --
    // with no intermediate vector.
    int dot = std::transform_reduce(a.begin(), a.end(), b.begin(), 0);
    std::cout << "dot product:    " << dot << '\n';   // 10 + 40 + 90 + 160 = 300

    // The unary form maps each element, then reduces. Arguments: range, init, the
    // reduce op (+), the transform op. Here: the sum of squares.
    int sumSquares = std::transform_reduce(a.begin(), a.end(), 0,
        std::plus<int>{}, [](int x){ return x * x; });
    std::cout << "sum of squares: " << sumSquares << '\n';   // 1 + 4 + 9 + 16 = 30
    return 0;
}
