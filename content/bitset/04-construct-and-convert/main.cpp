// A bitset builds from an integer or a string, and converts back with to_ulong /
// to_ullong / to_string. The integer conversions throw overflow_error when a set
// bit is wider than the target integer -- they do not truncate.

#include <bitset>
#include <iostream>
#include <stdexcept>

int main() {
    std::bitset<8> fromInt(42);          // low bits of 42
    std::bitset<8> fromStr("00101010");  // same pattern from text

    std::cout << "from int 42: " << fromInt << '\n';
    std::cout << "equal to the string one? " << std::boolalpha
              << (fromInt == fromStr) << '\n';
    std::cout << "back to number: " << fromInt.to_ulong() << '\n';
    std::cout << "as string:      " << fromInt.to_string() << '\n';

    // A value too wide for any integer: bit 70 is set.
    std::bitset<80> wide;
    wide.set(70);
    try {
        unsigned long long v = wide.to_ullong();   // 2^70 does not fit in 64 bits
        std::cout << v << '\n';
    } catch (const std::overflow_error&) {
        std::cout << "to_ullong threw overflow_error (no silent truncation)\n";
    }
    std::cout << "to_string still works, length " << wide.to_string().size() << '\n';
}
