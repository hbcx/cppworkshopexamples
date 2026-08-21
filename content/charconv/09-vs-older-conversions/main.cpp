#include <array>
#include <charconv>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>

int main() {
    int value = 1234;

    // 1) charconv: no allocation, no throw, no locale -- you own the buffer.
    std::array<char, 16> buf{};
    auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), value);
    (void)ec;
    std::cout << "to_chars:      " << std::string_view(buf.data(), ptr - buf.data()) << '\n';

    // 2) std::to_string: easy, but allocates a std::string every call.
    std::cout << "to_string:     " << std::to_string(value) << '\n';

    // 3) ostringstream: flexible, but allocates and follows the global locale.
    std::ostringstream oss;
    oss << value;
    std::cout << "ostringstream: " << oss.str() << '\n';

    // 4) snprintf: fast, no allocation, but the format string is not type-
    //    checked -- a wrong %-specifier is undefined behaviour.
    char c[16];
    std::snprintf(c, sizeof c, "%d", value);
    std::cout << "snprintf:      " << c << '\n';

    // Parsing back shows the same spread of trade-offs.
    std::string_view text = "1234";
    int back = 0;
    std::from_chars(text.data(), text.data() + text.size(), back);  // no throw, no alloc
    int viaStoi = std::stoi(std::string(text));                     // allocates, throws on bad input
    std::cout << "from_chars=" << back << "  stoi=" << viaStoi << '\n';
    return 0;
}
