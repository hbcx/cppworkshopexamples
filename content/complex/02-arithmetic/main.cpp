#include <complex>
#include <iostream>

int main() {
    std::complex<double> a(1.0, 2.0);   // 1 + 2i
    std::complex<double> b(3.0, 4.0);   // 3 + 4i

    std::cout << "a     = " << a << "\n";
    std::cout << "b     = " << b << "\n";
    std::cout << "a + b = " << (a + b) << "\n";   // (4,6)
    std::cout << "a - b = " << (a - b) << "\n";   // (-2,-2)
    std::cout << "a * b = " << (a * b) << "\n";   // (1*3-2*4, 1*4+2*3) = (-5,10)
    std::cout << "a / b = " << (a / b) << "\n";   // (11/25, 2/25) = (0.44,0.08)

    // Mixing with a real scalar works: the scalar acts as (s, 0).
    std::cout << "2.0 * a = " << (2.0 * a) << "\n";   // (2,4)
    std::cout << "a + 1.0 = " << (a + 1.0) << "\n";   // (2,2)
    return 0;
}
