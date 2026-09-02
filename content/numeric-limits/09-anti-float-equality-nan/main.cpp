#include <limits>
#include <iostream>
#include <cmath>

int main() {
    std::cout << std::boolalpha;

    // TRAP 1: comparing computed floats with == almost never holds, because the
    // results are rounded. 0.1 + 0.2 is not exactly 0.3.
    std::cout << "(0.1 + 0.2 == 0.3) ? " << (0.1 + 0.2 == 0.3) << "\n";   // false

    // TRAP 2: NaN compares UNEQUAL to everything -- including another NaN (and
    // even itself). So an == check to "detect NaN" silently fails.
    double nan1 = std::numeric_limits<double>::quiet_NaN();
    double nan2 = std::numeric_limits<double>::quiet_NaN();
    std::cout << "(nan == nan) ? " << (nan1 == nan2) << "\n";             // false
    std::cout << "(nan != nan) ? " << (nan1 != nan2) << "\n";             // true

    // FIX: test for NaN with std::isnan, and compare values with a tolerance.
    std::cout << "std::isnan(nan) ? " << std::isnan(nan1) << "\n";        // true
    double diff = std::fabs((0.1 + 0.2) - 0.3);
    std::cout << "close enough (|a-b| < 1e-9) ? " << (diff < 1e-9) << "\n"; // true
}
