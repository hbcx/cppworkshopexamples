#include <cmath>
#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::setprecision(6);

    std::cout << "sqrt(2)     = " << std::sqrt(2.0)       << "\n";
    std::cout << "cbrt(27)    = " << std::cbrt(27.0)      << "\n";
    std::cout << "pow(2, 10)  = " << std::pow(2.0, 10.0)  << "\n";
    std::cout << "pow(9, 0.5) = " << std::pow(9.0, 0.5)   << "   (a square root)\n";

    // hypot(x, y) is sqrt(x*x + y*y) -- the length of a 2D vector -- but it is
    // computed WITHOUT forming the squares that can overflow or underflow.
    std::cout << "hypot(3, 4) = " << std::hypot(3.0, 4.0) << "\n";

    // The overflow hypot avoids: 1e200 squared is 1e400, past double's range,
    // so the manual formula collapses to inf while hypot stays correct.
    double big = 1e200;
    std::cout << "\nfor a large input (1e200):\n";
    std::cout << "sqrt(big*big + big*big) = " << std::sqrt(big*big + big*big) << "\n";
    std::cout << "hypot(big, big)         = " << std::hypot(big, big)         << "\n";
    return 0;
}
