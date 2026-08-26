// Three different failures need three different tools:
//   * a BUG (a precondition your own code must satisfy) -> assert: catch it in
//     debug, and it compiles out in release.
//   * an EXPECTED runtime failure the caller handles -> return an error_code (or
//     std::optional/expected): cheap, explicit, no throw.
//   * an error that must NOT be ignored and may travel far -> throw an exception.
// The same function can use more than one: assert its internal assumptions, and
// report the caller-facing failure as a value or an exception.
#include <iostream>
#include <string>
#include <system_error>
#include <cassert>

// Parse a percentage 0..100. Bad text is an expected user error -> error_code.
// The internal invariant (digit-only text cannot be negative) is a bug if it
// ever fails -> assert.
std::error_code parse_percent(const std::string& text, int& out) {
    if (text.empty()) return std::make_error_code(std::errc::invalid_argument);
    for (char c : text) {
        if (c < '0' || c > '9') return std::make_error_code(std::errc::invalid_argument);
    }
    int value = std::stoi(text);
    assert(value >= 0 && "digit-only text cannot parse negative");   // internal invariant
    if (value > 100) return std::make_error_code(std::errc::result_out_of_range);
    out = value;
    return {};
}

int main() {
    std::cout << std::boolalpha;
    int pct = 0;

    std::error_code ec = parse_percent("75", pct);
    std::cout << "'75'  -> ok=" << !ec << " value=" << pct << '\n';

    ec = parse_percent("abc", pct);
    std::cout << "'abc' -> invalid_argument=" << (ec == std::errc::invalid_argument) << '\n';

    ec = parse_percent("250", pct);
    std::cout << "'250' -> out_of_range=" << (ec == std::errc::result_out_of_range) << '\n';
    return 0;
}
