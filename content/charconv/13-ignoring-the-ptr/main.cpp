#include <charconv>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>

int main() {
    // Anti-pattern: checking only ec and ignoring ptr. from_chars succeeds for
    // any leading number, so "12abc", "3.14xyz" and "5;" all look valid.

    // WRONG: accept a field whenever ec is success.
    auto parse_ec_only = [](std::string_view s) -> std::optional<int> {
        int v = 0;
        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), v);
        (void)ptr;
        return ec == std::errc{} ? std::optional<int>(v) : std::nullopt;
    };

    // RIGHT: also require ptr to reach the end -- the WHOLE field is a number.
    auto parse_whole = [](std::string_view s) -> std::optional<int> {
        int v = 0;
        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), v);
        if (ec == std::errc{} && ptr == s.data() + s.size())
            return v;
        return std::nullopt;
    };

    for (std::string_view s : {"42", "12abc", "5;"}) {
        auto a = parse_ec_only(s);
        auto b = parse_whole(s);
        std::cout << "\"" << s << "\"\tec-only: "
                  << (a ? std::to_string(*a) : "reject")
                  << "\twhole-field: "
                  << (b ? std::to_string(*b) : "reject") << '\n';
    }
    return 0;
}
