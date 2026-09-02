#include <cmath>
#include <iostream>
#include <iomanip>

// TRAP: computing a root with pow and a fractional exponent, e.g.
// pow(x, 1.0/3.0) for a cube root. pow works through exp(y * log(x)), so the
// exponent 1.0/3.0 is already inexact and the result picks up rounding error --
// the cube root of 1000 comes out as 9.999999... instead of exactly 10.
// std::cbrt is built for this and is both exact here and faster.
int main() {
    std::cout << std::setprecision(17);

    volatile double vx = 1000.0;    // volatile: force a run-time libm call, not
    double x = vx;                  // a compile-time fold that could differ per compiler
    double viaPow  = std::pow(x, 1.0 / 3.0);
    double viaCbrt = std::cbrt(x);

    std::cout << "pow(1000, 1.0/3.0) = " << viaPow  << "\n";
    std::cout << "cbrt(1000)         = " << viaCbrt << "\n";
    std::cout << "pow gives exactly 10 : " << std::boolalpha << (viaPow == 10.0) << "\n";
    std::cout << "cbrt gives exactly 10: " << (viaCbrt == 10.0) << "\n";

    std::cout << "\nAlso for simple powers: prefer x*x over pow(x, 2) -- it is\n"
                 "exact and far cheaper. Reserve pow for genuinely non-integer,\n"
                 "non-root exponents.\n";
    return 0;
}
