#include <iostream>
#include <compare>

// Celsius wraps an int and converts from one on purpose. With a single defaulted
// <=>, every relational operator works, and -- thanks to reversed candidates --
// the plain int can sit on either side of the comparison.

struct Celsius {
    int degrees;

    Celsius(int d) : degrees(d) {}          // intentionally implicit

    auto operator<=>(const Celsius&) const = default;
    bool operator==(const Celsius&) const = default;
};

int main() {
    Celsius t{20};

    // a < b is evaluated as (a <=> b) < 0; all four relational forms come from
    // the one operator:
    std::cout << "t <  25? " << (t < 25)  << "\n";   // 1
    std::cout << "t >  15? " << (t > 15)  << "\n";   // 1
    std::cout << "t <= 20? " << (t <= 20) << "\n";   // 1

    // Reversed candidate: 25 does not have a <=> for Celsius, so 25 < t is
    // evaluated as 0 < (t <=> 25), with the 25 converted to Celsius. A member
    // operator< in C++17 could not make the LEFT operand convert like this.
    std::cout << "25 < t?  " << (25 < t)  << "\n";   // 0
    std::cout << "15 < t?  " << (15 < t)  << "\n";   // 1
    std::cout << "20 == t? " << (20 == t) << "\n";   // 1
    return 0;
}
