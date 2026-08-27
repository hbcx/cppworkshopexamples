#include <iostream>
#include <cstdint>

// Anti-pattern: initializing with = or () across numeric types, which silently
// narrows and loses data. The value looks assigned but is quietly wrong. Brace
// init {} would have turned the same line into a compile error.

std::uint8_t clampByteBad(int value) {
    // Looks like it stores the value; actually keeps only the low 8 bits.
    std::uint8_t out = value;    // 300 -> 44, silently
    return out;
}

int main() {
    // A configuration value read as int, then squeezed into a byte:
    int configured = 300;
    std::cout << "byte from 300 (silent): "
              << static_cast<int>(clampByteBad(configured)) << "\n";   // 44

    // A fractional result truncated on the way into an int:
    double average = 4.9;
    int roundedWrong = average;     // 4, the .9 is gone
    std::cout << "int = 4.9 (silent):     " << roundedWrong << "\n";   // 4

    // THE FIX: braces reject the lossy conversion at compile time --
    //
    //     std::uint8_t out{value};   // error: narrowing int -> uint8_t
    //     int r{average};            // error: narrowing double -> int
    //
    // so you must say what you mean:
    int roundedOnPurpose{static_cast<int>(average + 0.5)};   // 5
    std::cout << "int{(int)(4.9+0.5)}:    " << roundedOnPurpose << "\n"; // 5

    // For a value that genuinely fits, braces are silent and safe:
    std::uint8_t small{200};
    std::cout << "byte{200}:              " << static_cast<int>(small) << "\n"; // 200
    return 0;
}
