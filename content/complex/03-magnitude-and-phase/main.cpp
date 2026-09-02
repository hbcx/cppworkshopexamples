#include <complex>
#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::setprecision(6);
    std::complex<double> z(3.0, 4.0);   // 3 + 4i

    std::cout << "z       = " << z          << "\n";
    std::cout << "abs(z)  = " << std::abs(z) << "   (modulus = sqrt(3^2 + 4^2))\n";  // 5
    std::cout << "norm(z) = " << std::norm(z)<< "   (|z|^2 = 25, no sqrt)\n";        // 25
    std::cout << "arg(z)  = " << std::arg(z) << "   (phase angle in radians)\n";     // 0.927295
    std::cout << "conj(z) = " << std::conj(z)<< "   (mirror across the real axis)\n";// (3,-4)

    std::cout << "\nabs and arg together are the polar form; norm is the cheap\n"
                 "squared magnitude, and conj flips the sign of the imaginary part.\n";
    return 0;
}
