#include <array>
#include <charconv>
#include <iostream>
#include <string_view>

int main() {
    // Both to_chars and from_chars take an optional base, 2..36 (default 10).
    // This is for integers only -- floating point uses chars_format instead.
    std::array<char, 32> buf{};
    int n = 255;

    for (int base : {2, 8, 16, 36}) {
        auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), n, base);
        (void)ec;
        std::string_view text(buf.data(), ptr - buf.data());
        std::cout << "255 in base " << base << ": " << text << '\n';
    }

    // Reading back: give from_chars the SAME base. Note there is no "0x" prefix
    // -- pass just the digits.
    std::string_view hex = "ff";
    int back = 0;
    std::from_chars(hex.data(), hex.data() + hex.size(), back, 16);
    std::cout << "\"ff\" read as base 16: " << back << '\n';

    // Letters are case-insensitive; digits above 9 run a..z / A..Z.
    std::string_view b36 = "Z";
    int z = 0;
    std::from_chars(b36.data(), b36.data() + b36.size(), z, 36);
    std::cout << "\"Z\" read as base 36:  " << z << '\n';
    return 0;
}
