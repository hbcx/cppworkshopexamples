#include <array>
#include <charconv>
#include <iostream>
#include <string_view>

int main() {
    std::array<char, 64> buf{};
    double x = 12345.6789;

    // Format x with the given arguments and print the written text. The extra
    // arguments are forwarded to to_chars: nothing, a chars_format, or a
    // chars_format plus a precision.
    auto show = [&](std::string_view label, auto... args) {
        auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), x, args...);
        (void)ec;
        std::cout << label << std::string_view(buf.data(), ptr - buf.data()) << '\n';
    };

    show("default (shortest):  ");
    show("fixed:               ", std::chars_format::fixed);
    show("fixed, precision 2:  ", std::chars_format::fixed, 2);
    show("scientific:          ", std::chars_format::scientific);
    show("scientific, prec 3:  ", std::chars_format::scientific, 3);
    show("general:             ", std::chars_format::general);
    show("hex:                 ", std::chars_format::hex);
    return 0;
}
