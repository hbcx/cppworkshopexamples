#include <limits>
#include <iostream>
#include <cstdint>

int main() {
    std::cout << std::boolalpha;

    // Ranges and properties, queried instead of hard-coded. The unary + prints
    // the char value as a number rather than a character.
    std::cout << "unsigned char: max=" << +std::numeric_limits<unsigned char>::max()
              << " digits=" << std::numeric_limits<unsigned char>::digits
              << " is_signed=" << std::numeric_limits<unsigned char>::is_signed << "\n";

    std::cout << "int:   max=" << std::numeric_limits<int>::max()
              << " digits=" << std::numeric_limits<int>::digits   // value bits, no sign
              << " is_signed=" << std::numeric_limits<int>::is_signed << "\n";

    std::cout << "int64: max=" << std::numeric_limits<std::int64_t>::max()
              << " digits=" << std::numeric_limits<std::int64_t>::digits << "\n";

    // is_modulo: unsigned arithmetic wraps in a defined way; signed overflow is
    // undefined, so signed types report false.
    std::cout << "unsigned is_modulo=" << std::numeric_limits<unsigned>::is_modulo
              << ", int is_modulo=" << std::numeric_limits<int>::is_modulo << "\n";
}
