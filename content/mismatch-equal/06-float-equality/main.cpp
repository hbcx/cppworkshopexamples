#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

int main() {
    std::cout << std::boolalpha;

    // These two ranges are "the same" mathematically, but built by different
    // arithmetic, so rounding leaves tiny differences (0.1 + 0.2 != 0.3).
    std::vector<double> computed{0.1 + 0.2, 1.0 / 3.0, 0.3 * 3.0};
    std::vector<double> expected{0.3, 0.333333333333333315, 0.9};

    // ANTI-PATTERN: std::equal with the default ==, which compares floating-point
    // values EXACTLY. The rounding error is enough to make them "not equal".
    bool exact = std::equal(computed.begin(), computed.end(), expected.begin(),
                            expected.end());
    std::cout << "equal with exact ==? " << exact << "   <- surprising\n";

    // FIX: pass a predicate that accepts values within a small tolerance. Compare
    // the difference against an epsilon instead of demanding bit-for-bit equality.
    const double eps = 1e-9;
    bool close = std::equal(computed.begin(), computed.end(), expected.begin(),
                            expected.end(),
                            [eps](double a, double b) {
                                return std::fabs(a - b) <= eps;
                            });
    std::cout << "equal within 1e-9?  " << close << '\n';

    // The same predicate idea works with mismatch to find the first element that
    // differs by MORE than the tolerance.
    return 0;
}
