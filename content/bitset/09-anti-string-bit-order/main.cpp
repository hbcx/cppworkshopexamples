// Anti-pattern: assuming the string constructor reads left-to-right as bit 0
// upward.
//
// The string is read like a number: the LEFTMOST character is the highest bit,
// the rightmost is bit 0. So "1000" sets bit 3, not bit 0.

#include <bitset>
#include <iostream>

int main() {
    std::bitset<4> b("1000");   // the '1' is the leftmost char = highest bit

    std::cout << std::boolalpha;
    std::cout << "bitset(\"1000\"): bit 3 = " << b[3]
              << ", bit 0 = " << b[0] << '\n';   // bit 3 set, bit 0 clear

    // GOOD 1: to set bit 0, put the 1 on the RIGHT (number order).
    std::bitset<4> c("0001");
    std::cout << "bitset(\"0001\"): bit 0 = " << c[0] << '\n';   // bit 0 set

    // GOOD 2: skip the string and set bits by index.
    std::bitset<4> d;
    d.set(0);
    std::cout << "set(0):        bit 0 = " << d[0] << '\n';
}
