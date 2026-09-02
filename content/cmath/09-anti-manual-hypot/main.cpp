#include <cmath>
#include <iostream>
#include <iomanip>

// TRAP: computing a 2D length as sqrt(x*x + y*y) by hand. Mathematically fine,
// numerically fragile: for a large x, x*x overflows to infinity long before the
// true length would, and for a tiny x it underflows to zero. std::hypot scales
// the inputs so the intermediate never overflows, giving the right answer over
// the whole range.
int main() {
    std::cout << std::setprecision(6);

    double x = 1e200, y = 1e200;   // true length ~ 1.41e200, well within range

    double manual = std::sqrt(x * x + y * y);   // x*x = 1e400 -> inf
    double safe   = std::hypot(x, y);

    std::cout << "sqrt(x*x + y*y) = " << manual << "   (overflowed to inf)\n";
    std::cout << "hypot(x, y)     = " << safe   << "   (correct)\n";

    // Same failure at the small end: squaring a tiny number underflows to 0.
    double tiny = 1e-200;
    std::cout << "\nsqrt(tiny^2 + tiny^2) = " << std::sqrt(tiny*tiny + tiny*tiny)
              << "   (underflowed to 0)\n";
    std::cout << "hypot(tiny, tiny)     = " << std::hypot(tiny, tiny)
              << "   (correct)\n";

    std::cout << "\nFIX: use std::hypot for any distance or vector length.\n";
    return 0;
}
