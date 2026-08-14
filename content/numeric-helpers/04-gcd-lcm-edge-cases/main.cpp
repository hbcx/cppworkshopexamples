#include <iostream>
#include <numeric>

int main() {
    // EDGE CASE 1: gcd(0, 0) is defined to be 0. That is fine until you use the
    // result as a divisor -- reducing a fraction by dividing through the gcd -- at
    // which point 0 means a division by zero.
    int a = 0, b = 0;
    int g = std::gcd(a, b);
    std::cout << "gcd(0, 0) = " << g << '\n';
    if (g == 0) {
        std::cout << "  guard: gcd is 0, cannot divide by it\n";   // handle before /
    }

    // EDGE CASE 2: lcm with a zero argument is 0 (the least common multiple of
    // anything and 0 is 0). Using it as a common denominator would again give 0.
    std::cout << "lcm(6, 0) = " << std::lcm(6, 0) << '\n';

    // ANTI-PATTERN: assuming lcm always fits. lcm(a, b) is a / gcd(a, b) * b, and
    // for large coprime values that product OVERFLOWS the integer type -- undefined
    // behaviour for signed types, so we do NOT compute it here:
    //
    //   std::lcm(1000000000, 999999999);   // ~1e18, overflows 32-bit int -> UB
    //
    // FIX: use a wide enough type, and/or check that a / gcd(a,b) will not overflow
    // when multiplied by b. Here a 64-bit result holds it safely.
    long long x = 1000000000LL, y = 999999999LL;
    std::cout << "lcm as long long = " << std::lcm(x, y) << '\n';
    return 0;
}
