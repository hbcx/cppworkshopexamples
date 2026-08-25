#include <iomanip>
#include <iostream>

int main() {
    // boolalpha prints bools as true/false instead of 1/0.
    std::cout << "default bool: " << true << " " << false << '\n';   // 1 0
    std::cout << "boolalpha:    " << std::boolalpha << true << " " << false << '\n'; // true false

    // showpos adds a leading + to non-negative numbers.
    std::cout << "showpos: " << std::showpos << 42 << " " << -42 << '\n';   // +42 -42
    std::cout << std::noshowpos;

    // showpoint keeps the decimal point and trailing zeros on whole floats.
    std::cout << "showpoint: " << std::showpoint << std::setprecision(4) << 3.0 << '\n'; // 3.000
    return 0;
}
