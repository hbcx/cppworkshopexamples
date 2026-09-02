#include <complex>
#include <iostream>

int main() {
    using namespace std::complex_literals;   // brings in the i / if / il suffixes

    std::cout << std::boolalpha;

    // The i suffix makes a std::complex<double> with the given imaginary part,
    // so expressions read like the math: 1 + 2i.
    auto z = 1.0 + 2.0i;
    std::cout << "1.0 + 2.0i   = " << z << "\n";      // (1,2)

    // if -> complex<float>, il -> complex<long double>.
    auto zf = 1.0f + 2.0if;
    std::cout << "1.0f + 2.0if = " << zf << "\n";     // (1,2)

    // A bare literal is pure imaginary: 3i is (0,3).
    std::cout << "3.0i         = " << 3.0i << "\n";   // (0,3)

    // Same value as the constructor form, just more readable.
    std::complex<double> verbose(1.0, 2.0);
    std::cout << "literal == constructor : " << (z == verbose) << "\n";  // true
    return 0;
}
