#include <cmath>
#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::fixed << std::setprecision(1);

    const double vals[] = { 2.3, 2.5, 2.7, -2.3, -2.5, -2.7 };

    std::cout << "value  floor  ceil  round  trunc  nearbyint\n";
    for (double v : vals) {
        std::cout << std::setw(5) << v << "  "
                  << std::setw(5) << std::floor(v)     << "  "
                  << std::setw(4) << std::ceil(v)      << "  "
                  << std::setw(5) << std::round(v)     << "  "
                  << std::setw(5) << std::trunc(v)     << "  "
                  << std::setw(9) << std::nearbyint(v) << "\n";
    }

    std::cout << "\nfloor -> down, ceil -> up, trunc -> toward zero,\n"
                 "round -> nearest, ties away from zero (2.5 -> 3, -2.5 -> -3),\n"
                 "nearbyint -> nearest, ties to EVEN (2.5 -> 2, -2.5 -> -2).\n";
    return 0;
}
