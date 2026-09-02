#include <complex>
#include <cmath>
#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::setprecision(6);
    volatile double vpi = std::acos(-1.0);
    double pi = vpi;
    using cd = std::complex<double>;

    // sqrt of a negative real is imaginary: sqrt(-1) = i.
    std::cout << "sqrt(-1+0i) = " << std::sqrt(cd(-1.0, 0.0)) << "\n";   // (0,1)

    // Euler's identity: e^{i*pi} = -1 (the imaginary part is a tiny rounding).
    std::cout << "exp(i*pi)   = " << std::exp(cd(0.0, pi)) << "\n";      // (-1, ~1.2e-16)

    // log is the inverse of exp; pow raises a complex to a power.
    std::cout << "log(e+0i)   = " << std::log(cd(std::exp(1.0), 0.0)) << "\n";  // (1,0)
    std::cout << "(1+i)^2     = " << std::pow(cd(1.0, 1.0), 2.0) << "\n";       // (~0,2)

    std::cout << "\nThese are the complex overloads of the <cmath> functions: they\n"
                 "follow the branch cuts of the mathematical definitions.\n";
    return 0;
}
