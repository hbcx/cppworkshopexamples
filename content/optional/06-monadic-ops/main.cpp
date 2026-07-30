#include <iostream>
#include <optional>
#include <string>

std::optional<int> parsePort(const std::string& s) {
    if (s.empty()) return std::nullopt;
    int v = 0;
    for (char c : s) {
        if (c < '0' || c > '9') return std::nullopt;
        v = v * 10 + (c - '0');
    }
    return v;
}

int main() {
    // C++23 monadic operations chain fallible steps without nested if (has_value):
    //   and_then  -- run a step that itself returns an optional, and flatten
    //   transform -- map the value through a plain function, staying optional
    //   or_else   -- supply an alternative optional when empty
    auto describe = [](const std::string& in) {
        std::optional<std::string> out =
            parsePort(in)
                .and_then([](int p) -> std::optional<int> {
                    return p <= 65535 ? std::optional<int>(p) : std::nullopt;  // range-check
                })
                .transform([](int p) { return "port " + std::to_string(p); })
                .or_else([]() -> std::optional<std::string> { return "(invalid)"; });
        std::cout << "\"" << in << "\" -> " << *out << '\n';
    };

    describe("8080");    // parses and in range
    describe("99999");   // parses but out of range -> and_then drops it
    describe("nope");    // parse fails at the first step
    return 0;
}
