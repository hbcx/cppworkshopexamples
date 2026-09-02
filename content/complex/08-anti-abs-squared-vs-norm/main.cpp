#include <complex>
#include <iostream>
#include <iomanip>

// TRAP: writing abs(z)*abs(z) when you want the squared magnitude |z|^2. abs
// computes a sqrt internally; squaring it undoes that sqrt while paying for it,
// and the round-trip through sqrt adds a rounding error. std::norm gives
// re*re + im*im directly -- no sqrt, and exact where the squares are exact.
int main() {
    std::cout << std::setprecision(17);
    std::complex<double> z(1.0, 1.0);   // |z|^2 is exactly 2

    double viaAbs  = std::abs(z) * std::abs(z);   // sqrt(2) then square
    double viaNorm = std::norm(z);                // 1*1 + 1*1

    std::cout << "abs(z)*abs(z) = " << viaAbs  << "   (sqrt then square: rounding creeps in)\n";
    std::cout << "norm(z)       = " << viaNorm << "   (exact)\n";

    std::cout << "\nFIX: use std::norm for a squared magnitude. To COMPARE two\n"
                 "magnitudes, compare their norms -- same ordering, and no sqrt at all.\n";
    return 0;
}
