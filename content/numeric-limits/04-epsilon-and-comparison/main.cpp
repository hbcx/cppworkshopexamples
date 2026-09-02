#include <limits>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

// Near-equality with a relative tolerance scaled by the magnitudes, plus an
// absolute floor (epsilon) so values near zero still compare.
static bool nearly_equal(double a, double b, double rel = 1e-9) {
    double diff  = std::fabs(a - b);
    double scale = std::max(std::fabs(a), std::fabs(b));
    return diff <= std::max(rel * scale, std::numeric_limits<double>::epsilon());
}

int main() {
    std::cout << std::boolalpha;

    // 0.1 + 0.2 is not exactly 0.3 in binary floating point.
    double x = 0.1 + 0.2;
    std::cout << std::setprecision(17) << "0.1 + 0.2 = " << x << "\n";
    std::cout << "x == 0.3 ?          " << (x == 0.3) << "\n";              // false
    std::cout << "nearly_equal(x,0.3) " << nearly_equal(x, 0.3) << "\n";   // true

    std::cout << "epsilon = " << std::numeric_limits<double>::epsilon() << "\n";
}
