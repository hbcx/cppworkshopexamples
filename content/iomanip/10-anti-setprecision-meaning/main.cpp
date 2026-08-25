#include <iomanip>
#include <iostream>

int main() {
    // Anti-pattern: assuming setprecision(2) always means "2 decimal places".
    // Without fixed (or scientific), precision is the number of SIGNIFICANT
    // digits, so a large value comes out in scientific form, not two decimals.
    double value = 1234.5;
    std::cout << "wrong (no fixed): "
              << std::setprecision(2) << value << '\n';   // 1.2e+03

    // Fix: set std::fixed, and THEN setprecision counts digits after the point.
    std::cout << "right (fixed):    "
              << std::fixed << std::setprecision(2) << value << '\n';   // 1234.50
    return 0;
}
