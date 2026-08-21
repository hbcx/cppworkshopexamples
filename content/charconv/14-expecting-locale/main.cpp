#include <array>
#include <charconv>
#include <iostream>
#include <string_view>
#include <system_error>

int main() {
    // Anti-pattern: expecting charconv to follow the locale -- a decimal comma,
    // or grouped thousands. It NEVER does: the decimal point is always '.' and
    // there is no grouping. This is by design (portable output), but it
    // surprises people coming from stringstream / printf.

    // 1) A German-style "3,14" does not parse as 3.14: from_chars reads '3'
    //    and stops at the comma.
    std::string_view german = "3,14";
    double v = 0;
    auto [ptr, ec] = std::from_chars(german.data(), german.data() + german.size(), v);
    bool whole = ec == std::errc{} && ptr == german.data() + german.size();
    std::cout << "\"3,14\" parsed as " << v
              << (whole ? " (all)\n" : " (stopped at the comma)\n");

    // 2) to_chars never inserts thousands separators.
    std::array<char, 32> buf{};
    auto [p2, e2] = std::to_chars(buf.data(), buf.data() + buf.size(), 1000000);
    (void)e2;
    std::cout << "one million: " << std::string_view(buf.data(), p2 - buf.data())
              << " (no grouping)\n";

    // The fix: charconv is the RIGHT tool for machine-readable formats. For a
    // localized string meant for a human, use std::format with the L option
    // (or the <locale> facets).
    return 0;
}
