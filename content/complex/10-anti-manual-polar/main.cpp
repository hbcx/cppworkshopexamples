#include <complex>
#include <cmath>
#include <iostream>
#include <iomanip>

// TRAP: building a complex from a magnitude and an angle by hand:
//   z = r*cos(theta) + r*sin(theta)*i
// It works, but it is easy to swap cos and sin, forget the r on one term, or
// get a sign wrong -- and it buries the intent. std::polar(r, theta) does
// exactly this, correctly and readably.
int main() {
    std::cout << std::setprecision(6);
    volatile double vpi = std::acos(-1.0);
    double theta = vpi / 3;    // 60 degrees
    double r = 2.0;

    std::complex<double> byHand(r * std::cos(theta), r * std::sin(theta));
    std::complex<double> viaPolar = std::polar(r, theta);

    std::cout << std::boolalpha;
    std::cout << "by hand     = " << byHand   << "\n";
    std::cout << "std::polar  = " << viaPolar << "\n";
    std::cout << "same result = " << (byHand == viaPolar) << "\n";

    std::cout << "\nFIX: std::polar(r, theta) says what it means and cannot get the\n"
                 "cos/sin order or a sign wrong. Reach for it given a magnitude and angle.\n";
    return 0;
}
