#include <complex>
#include <cmath>
#include <iostream>
#include <iomanip>

// TRAP: testing a complex result for exact equality after transcendental
// arithmetic. Euler's identity says e^{i*pi} = -1, but in floating point the
// imaginary part comes out a tiny nonzero rounding, so exp(i*pi) == -1 is false.
// Both components carry error; compare the DISTANCE between the values against a
// tolerance instead.
int main() {
    std::cout << std::setprecision(17);
    volatile double vpi = std::acos(-1.0);
    double pi = vpi;
    using cd = std::complex<double>;

    cd result = std::exp(cd(0.0, pi));   // ~ -1, but not exactly
    cd expected(-1.0, 0.0);

    std::cout << std::boolalpha;
    std::cout << "exp(i*pi)           = " << result << "\n";
    std::cout << "result == -1        = " << (result == expected) << "\n";   // false

    // FIX: is the distance between them below a tolerance? abs of the difference
    // is the magnitude of the gap, covering both components at once.
    double dist = std::abs(result - expected);
    std::cout << "abs(result - (-1))  = " << dist << "\n";
    std::cout << "close enough (<1e-12) = " << (dist < 1e-12) << "\n";        // true
    return 0;
}
