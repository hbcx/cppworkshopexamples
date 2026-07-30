#include <expected>
#include <iostream>
#include <string>

// A rich error type: not just "it failed" but what and where.
struct ParseError {
    int position;
    std::string message;
};

std::expected<int, ParseError> parseIndex(const std::string& s) {
    if (s.empty()) return std::unexpected(ParseError{0, "empty input"});
    int value = 0;
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        char c = s[static_cast<std::size_t>(i)];
        if (c < '0' || c > '9')
            return std::unexpected(ParseError{i, std::string("not a digit: ") + c});
        value = value * 10 + (c - '0');
    }
    return value;
}

int main() {
    for (const std::string& in : {std::string("123"), std::string("1x9"), std::string("")}) {
        auto r = parseIndex(in);
        if (r) {
            std::cout << "\"" << in << "\" -> " << *r << '\n';
        } else {
            std::cout << "\"" << in << "\" -> error at " << r.error().position
                      << ": " << r.error().message << '\n';
        }
    }
    return 0;
}
