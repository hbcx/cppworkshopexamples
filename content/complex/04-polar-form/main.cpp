#include <complex>
#include <cmath>
#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::setprecision(6);
    volatile double vpi = std::acos(-1.0);   // volatile: run-time value, no fold
    double pi = vpi;

    // polar(r, theta) builds r * (cos theta + i sin theta) = r * e^{i theta}.
    std::complex<double> z = std::polar(2.0, pi / 4);   // length 2, angle 45 deg
    std::cout << "polar(2, pi/4) = " << z << "\n";       // (1.41421,1.41421)

    // Recover the polar coordinates from the Cartesian value.
    std::cout << "abs = " << std::abs(z) << ", arg = " << std::arg(z) << "\n";  // 2, 0.785398

    // Euler: e^{i theta} has length 1 and angle theta, so polar(1, pi/2) ~ i.
    std::complex<double> u = std::polar(1.0, pi / 2);
    std::cout << "polar(1, pi/2) = " << u << "   (~ i, tiny real part is rounding)\n";
    return 0;
}
