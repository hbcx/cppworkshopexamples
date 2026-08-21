#include <charconv>
#include <cstddef>
#include <iostream>
#include <string_view>
#include <system_error>

int main() {
    // Anti-pattern: expecting from_chars to behave like strtol / stoi. It does
    // NOT skip leading whitespace, does NOT accept a leading '+', and does NOT
    // recognise a "0x" prefix.
    auto parse = [](std::string_view s, int base = 10) {
        int v = 0;
        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), v, base);
        std::cout << "\"" << s << "\" -> ";
        if (ec != std::errc{}) {
            std::cout << "invalid_argument\n";
            return;
        }
        bool whole = ptr == s.data() + s.size();
        std::cout << "value " << v << (whole ? " (all consumed)\n" : " (stopped early)\n");
    };

    parse(" 42");        // leading space  -> invalid_argument
    parse("+42");        // leading '+'    -> invalid_argument
    parse("0x1f", 16);   // "0x": reads the 0, stops at 'x' -> value 0, stopped early
    parse("1f", 16);     // the bare digits, base 16 -> ok

    // The fix: strip whitespace and sign yourself, then call from_chars.
    std::string_view raw = "  +42";
    std::size_t i = raw.find_first_not_of(' ');
    bool neg = false;
    if (i != std::string_view::npos && (raw[i] == '+' || raw[i] == '-')) {
        neg = raw[i] == '-';
        ++i;
    }
    int v = 0;
    std::from_chars(raw.data() + i, raw.data() + raw.size(), v);
    std::cout << "after trimming \"  +42\": " << (neg ? -v : v) << '\n';
    return 0;
}
