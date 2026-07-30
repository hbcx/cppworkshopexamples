#include <expected>
#include <iostream>
#include <string>

std::expected<int, std::string> parsePort(const std::string& s) {
    if (s.empty()) return std::unexpected("empty");
    int v = 0;
    for (char c : s) {
        if (c < '0' || c > '9') return std::unexpected("not a number");
        v = v * 10 + (c - '0');
    }
    return v;
}

int main() {
    // Monadic ops chain fallible steps, carrying the error through automatically:
    //   and_then        -- next step returns expected; short-circuit + flatten
    //   transform       -- map the value; an error passes through untouched
    //   transform_error -- map/translate the error; a value passes through
    auto run = [](const std::string& in) {
        std::expected<std::string, std::string> out =
            parsePort(in)
                .and_then([](int p) -> std::expected<int, std::string> {
                    if (p > 65535) return std::unexpected("out of range");
                    return p;
                })
                .transform([](int p) { return "port " + std::to_string(p); })
                .transform_error([](const std::string& e) { return "rejected (" + e + ")"; });
        if (out) std::cout << "\"" << in << "\" -> " << *out << '\n';
        else     std::cout << "\"" << in << "\" -> " << out.error() << '\n';
    };

    run("8080");     // ok all the way through
    run("99999");    // parses, but and_then rejects it as out of range
    run("nope");     // parse fails at the first step
    return 0;
}
