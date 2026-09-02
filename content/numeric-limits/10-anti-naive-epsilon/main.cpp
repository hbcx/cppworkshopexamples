#include <limits>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

int main() {
    std::cout << std::boolalpha;
    const double eps = std::numeric_limits<double>::epsilon();   // ~2.22e-16

    // Two doubles as close as two distinct doubles can be, but around 1e8.
    double a = 1e8;
    double b = std::nextafter(a, 2e8);   // the very next representable value up

    // TRAP: a FIXED absolute tolerance of epsilon. epsilon is the gap between
    // adjacent doubles AT 1.0; around 1e8 that gap is ~1.5e-8, far larger than
    // epsilon, so even ADJACENT values are called "not equal".
    std::cout << std::setprecision(3)
              << "gap between adjacent doubles at 1e8 = " << (b - a) << "\n";
    std::cout << "naive |a-b| < epsilon ? " << (std::fabs(a - b) < eps)
              << "  (wrong: adjacent values called unequal)\n";

    // FIX: scale the tolerance by the magnitude (a relative comparison).
    double scale = std::max(std::fabs(a), std::fabs(b));
    std::cout << "relative |a-b| <= epsilon*scale*4 ? "
              << (std::fabs(a - b) <= eps * scale * 4.0) << "  (correct)\n";
}
