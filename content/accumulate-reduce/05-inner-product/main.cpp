#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> a{1, 2, 3, 4};
    std::vector<int> b{10, 20, 30, 40};

    // std::inner_product is the classic sequential fold over TWO ranges. With the
    // defaults it multiplies matched elements and sums the products, starting from
    // an init -- a dot product. It reads the second range from first2 only, so
    // that range must be at least as long as the first.
    int dot = std::inner_product(a.begin(), a.end(), b.begin(), 0);
    std::cout << "dot product: " << dot << '\n';   // 10 + 40 + 90 + 160 = 300

    // The general form takes TWO operations: the outer "combine" (default +) and
    // the inner "pair up" (default *). Swap them to compute other things. Here:
    // count the positions where the two ranges are EQUAL -- inner op is ==, outer
    // op is +, so it sums a 1 for each matching pair.
    std::vector<int> x{1, 9, 3, 8};
    std::vector<int> y{1, 2, 3, 4};
    int matches = std::inner_product(x.begin(), x.end(), y.begin(), 0,
                                     std::plus<int>{},
                                     [](int p, int q) { return p == q ? 1 : 0; });
    std::cout << "matching positions: " << matches << '\n';   // index 0 and 2

    // The same two-custom-op shape builds a weighted sum, a similarity score, or
    // a max of pairwise sums. In C++17, transform_reduce is this done in parallel.
    return 0;
}
