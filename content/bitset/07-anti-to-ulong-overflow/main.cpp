// Anti-pattern: expecting to_ulong() to truncate to the low bits.
//
// If a set bit is wider than the target integer, to_ulong / to_ullong throw
// overflow_error -- they never return just the low word. Mask first if you want
// the low bits, or use to_string for the whole value.

#include <bitset>
#include <iostream>
#include <stdexcept>

int main() {
    std::bitset<80> wide;
    wide.set(2);
    wide.set(70);                    // a bit far beyond 64

    // BAD assumption: to_ullong() would hand back the low bits. It throws.
    try {
        unsigned long long v = wide.to_ullong();
        std::cout << v << '\n';
    } catch (const std::overflow_error&) {
        std::cout << "to_ullong threw overflow_error (no truncation)\n";
    }

    // GOOD: to_string always works, whatever the width.
    std::string text = wide.to_string();
    std::cout << "as string, length " << text.size()
              << ", set bits " << wide.count() << '\n';

    // GOOD: if you only want the low 8 bits, take them explicitly.
    unsigned long low = (wide & std::bitset<80>(0xFFu)).to_ulong();
    std::cout << "low 8 bits as a number: " << low << '\n';
}
