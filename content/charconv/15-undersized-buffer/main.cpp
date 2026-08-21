#include <charconv>
#include <iostream>
#include <limits>
#include <string_view>
#include <system_error>

int main() {
    // Anti-pattern: sizing the buffer for the "normal" case and forgetting the
    // worst case -- the sign plus the widest value.

    // INT_MIN is -2147483648: eleven characters including the '-'. A 10-byte
    // buffer holds every other int and overflows exactly on this one.
    int worst = std::numeric_limits<int>::min();

    char tooSmall[10];
    auto [p1, e1] = std::to_chars(tooSmall, tooSmall + sizeof tooSmall, worst);
    (void)p1;
    std::cout << "10-byte buffer for INT_MIN: "
              << (e1 == std::errc::value_too_large ? "value_too_large\n" : "ok\n");

    // Safe size for a base-10 integer type: digits10 undercounts by one, plus
    // room for a sign, plus a small margin. A round number like this is the
    // easy correct default.
    constexpr int safe = std::numeric_limits<int>::digits10 + 3;
    char big[safe];
    auto [p2, e2] = std::to_chars(big, big + sizeof big, worst);
    std::cout << "sized buffer (" << safe << " bytes): "
              << (e2 == std::errc{} ? std::string_view(big, p2 - big)
                                    : std::string_view("failed"))
              << '\n';

    // Floats need more room (scientific and hex forms are long), so char
    // buf[64] is a safe, generous default for any single number.
    return 0;
}
