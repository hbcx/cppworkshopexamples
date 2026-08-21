#include <charconv>
#include <iostream>
#include <string_view>
#include <system_error>

int main() {
    // from_chars parses floating point too. An optional chars_format says which
    // grammar to accept; the default (general) allows fixed OR scientific.
    // We require the WHOLE string to be consumed to count as accepted.
    auto parse = [](std::string_view s, std::chars_format fmt) {
        double v = 0;
        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), v, fmt);
        std::cout << "\"" << s << "\" -> ";
        if (ec == std::errc{} && ptr == s.data() + s.size())
            std::cout << v << '\n';
        else
            std::cout << "(rejected)\n";
    };

    parse("3.14", std::chars_format::general);      // ok: plain decimal
    parse("6.022e23", std::chars_format::general);  // ok: scientific
    parse("6.022e23", std::chars_format::fixed);    // rejected: 'e' not allowed
    parse("1.5", std::chars_format::scientific);    // rejected: needs an exponent
    parse("1a2p3", std::chars_format::hex);         // ok: hex float, no 0x prefix
    return 0;
}
