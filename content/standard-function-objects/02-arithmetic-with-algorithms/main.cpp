#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> a{1, 2, 3, 4};
    std::vector<int> b{10, 20, 30, 40};

    // std::accumulate's default operation already IS std::plus; a product needs
    // std::multiplies instead of writing a lambda for it.
    long product = std::accumulate(a.begin(), a.end(), 1L, std::multiplies<long>());
    std::cout << "product = " << product << "\n";   // 24

    // Combine two ranges elementwise with std::transform + std::plus.
    std::vector<int> sums(a.size());
    std::transform(a.begin(), a.end(), b.begin(), sums.begin(), std::plus<int>());
    std::cout << "elementwise a+b:";
    for (int x : sums) std::cout << " " << x;
    std::cout << "\n";   // 11 22 33 44

    // A dot product: std::inner_product's two operations default to plus and
    // multiplies -- the sum of a[i]*b[i].
    long dot = std::inner_product(a.begin(), a.end(), b.begin(), 0L);
    std::cout << "dot(a, b) = " << dot << "\n";   // 300

    // std::negate is the one unary arithmetic object -- flip every sign.
    std::vector<int> neg(a.size());
    std::transform(a.begin(), a.end(), neg.begin(), std::negate<int>());
    std::cout << "negated:";
    for (int x : neg) std::cout << " " << x;
    std::cout << "\n";   // -1 -2 -3 -4
    return 0;
}
