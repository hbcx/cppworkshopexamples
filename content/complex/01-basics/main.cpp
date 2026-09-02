#include <complex>
#include <iostream>

int main() {
    std::complex<double> z(3.0, 4.0);   // 3 + 4i

    std::cout << std::boolalpha;
    std::cout << "z        = " << z        << "   (printed as (real,imag))\n";
    std::cout << "real(z)  = " << z.real() << "\n";
    std::cout << "imag(z)  = " << z.imag() << "\n";

    // Value equality compares both components.
    std::complex<double> a(1.0, 2.0), b(1.0, 2.0), c(1.0, -2.0);
    std::cout << "a == b   = " << (a == b) << "\n";   // true
    std::cout << "a == c   = " << (a == c) << "\n";   // false (imag differs)

    // A real number is a complex with a zero imaginary part.
    std::complex<double> r(5.0);        // 5 + 0i
    std::cout << "r        = " << r << "\n";

    // real() and imag() also act as setters.
    z.real(7.0);
    z.imag(1.0);
    std::cout << "after setters = " << z << "\n";     // (7,1)
    return 0;
}
