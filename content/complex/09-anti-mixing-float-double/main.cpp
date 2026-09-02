#include <complex>
#include <iostream>
#include <iomanip>

// TRAP: mixing std::complex<float> and std::complex<double> in one expression.
// The operators are defined for a single value type T only, so there is no
// operator+ between complex<float> and complex<double> -- the line below does
// not compile:
//
//     std::complex<float>  f(1.0f, 2.0f);
//     std::complex<double> d(3.0, 4.0);
//     auto bad = f + d;        // ERROR: no matching operator+
//
// The fix is to convert one side to the other's value type first.
int main() {
    std::cout << std::setprecision(6);
    std::complex<float>  f(1.0f, 2.0f);
    std::complex<double> d(3.0, 4.0);

    // The converting constructor widens complex<float> to complex<double>.
    std::complex<double> promoted(f);
    std::cout << "promoted + d = " << (promoted + d) << "\n";   // (4,6)

    std::cout << "\nFIX: convert to a common value type before combining. And\n"
                 "prefer complex<double> by default -- complex<float> carries the\n"
                 "same precision loss as a bare float, compounded by every op.\n";
    return 0;
}
