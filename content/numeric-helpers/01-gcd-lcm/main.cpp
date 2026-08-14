#include <iostream>
#include <numeric>

int main() {
    // std::gcd (C++17) returns the greatest common divisor -- the largest number
    // that divides both. Its classic use is reducing a fraction to lowest terms:
    // divide numerator and denominator by their gcd.
    int num = 18, den = 24;
    int g = std::gcd(num, den);
    std::cout << "gcd(18, 24) = " << g << '\n';
    std::cout << "18/24 reduces to " << (num / g) << '/' << (den / g) << '\n';

    // std::lcm (C++17) returns the least common multiple -- the smallest number
    // both divide into. It answers "common denominator" and "when do two cycles
    // line up": two events every 4 and every 6 ticks coincide every lcm ticks.
    std::cout << "lcm(4, 6) = " << std::lcm(4, 6) << '\n';

    // Both accept mixed integer types and ignore the sign of their arguments
    // (the result is non-negative). gcd is the building block: lcm(a,b) is
    // computed as a / gcd(a,b) * b.
    std::cout << "gcd(-12, 18) = " << std::gcd(-12, 18) << '\n';
    return 0;
}
