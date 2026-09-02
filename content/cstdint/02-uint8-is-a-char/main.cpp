#include <cstdint>
#include <iostream>

// int8_t and uint8_t are almost always typedefs for signed char / unsigned
// char. That has one surprising effect on iostreams: << treats them as
// CHARACTERS, not numbers. std::cout << u prints the glyph whose code is the
// value, not the value itself.
int main() {
    std::uint8_t code = 65;

    std::cout << "printed directly : " << code << "\n";                  // 'A', not 65
    std::cout << "with unary plus  : " << +code << "\n";                 // 65 (+ promotes to int)
    std::cout << "cast to int      : " << static_cast<int>(code) << "\n";

    std::int8_t s = 66;
    std::cout << "\nint8_t behaves the same way:\n";
    std::cout << "printed directly : " << s << "\n";                     // 'B'
    std::cout << "with unary plus  : " << +s << "\n";                    // 66

    // The same trap hides in a std::string built from these bytes, in
    // std::cin >> u8 (it reads one character, not a number), and anywhere a
    // uint8_t reaches a << that has a char overload.
    std::cout << "\nrule: to see the NUMBER, promote with +x or cast to int.\n";
}
