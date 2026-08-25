#include <iomanip>
#include <iostream>

int main() {
    double pi = 3.14159265358979;
    double big = 1234567.0;

    // Default (general) format: setprecision sets SIGNIFICANT digits.
    std::cout << "default p4:   " << std::setprecision(4) << pi << '\n';   // 3.142

    // fixed: setprecision now means digits AFTER the decimal point.
    std::cout << "fixed p2:     " << std::fixed << std::setprecision(2) << pi << '\n';   // 3.14

    // scientific: mantissa + exponent, precision = digits after the point.
    std::cout << "scientific:   " << std::scientific << std::setprecision(2) << big << '\n'; // 1.23e+06

    // defaultfloat returns to the general format.
    std::cout << "defaultfloat: " << std::defaultfloat << std::setprecision(6) << big << '\n'; // 1.23457e+06
    return 0;
}
