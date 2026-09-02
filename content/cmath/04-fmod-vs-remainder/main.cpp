#include <cmath>
#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::fixed << std::setprecision(2);

    // fmod(a, b) is the remainder of a/b truncated toward zero, so its result
    // has the SIGN OF a and lies in (-|b|, |b|). remainder(a, b) rounds the
    // quotient to the NEAREST integer (ties to even), so its result is the one
    // closest to zero and lies in [-|b|/2, |b|/2] -- it can be negative even
    // when both inputs are positive.
    struct Case { double a, b; };
    const Case cases[] = { {5.3, 2.0}, {-5.3, 2.0}, {5.3, -2.0}, {7.0, 3.0} };

    std::cout << "   a      b    fmod   remainder\n";
    for (auto c : cases) {
        std::cout << std::setw(5) << c.a << "  " << std::setw(5) << c.b << "  "
                  << std::setw(5) << std::fmod(c.a, c.b) << "   "
                  << std::setw(5) << std::remainder(c.a, c.b) << "\n";
    }

    std::cout << "\nfmod keeps the sign of the dividend; remainder returns the\n"
                 "value nearest zero, so remainder(7, 3) is 1 but remainder(5.3, 2)\n"
                 "is negative because 5.3 is closer to 3*2 than to 2*2.\n";
    return 0;
}
