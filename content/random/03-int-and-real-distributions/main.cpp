#include <random>
#include <iostream>
#include <iomanip>

int main() {
    std::mt19937 gen(7);

    // uniform_int_distribution(a, b) covers the CLOSED range [a, b] -- both
    // ends included. So this yields 1, 2 or 3 (see the off-by-one trap later).
    std::uniform_int_distribution<int> ints(1, 3);
    std::cout << "ints in [1,3]:";
    for (int i = 0; i < 12; ++i) std::cout << ' ' << ints(gen);
    std::cout << "\n";

    // uniform_real_distribution(a, b) covers the HALF-OPEN range [a, b) -- b is
    // excluded, like most C++ ranges.
    std::uniform_real_distribution<double> reals(0.0, 1.0);   // [0, 1)
    std::cout << std::fixed << std::setprecision(3) << "reals in [0,1):";
    for (int i = 0; i < 5; ++i) std::cout << ' ' << reals(gen);
    std::cout << "\n";
}
