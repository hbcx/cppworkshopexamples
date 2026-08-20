#include <iostream>
#include <regex>
#include <string>

int main() {
    std::cout << std::boolalpha;

    // A malformed pattern is a RUNTIME failure: building the std::regex throws
    // std::regex_error. There is no compile-time check on the pattern text.
    try {
        std::regex bad("(unclosed");
        std::cout << "no error?\n";        // not reached
    } catch (const std::regex_error& e) {
        // Compare code() to a known constant -- this is portable, unlike what(),
        // whose message wording differs between standard library implementations.
        std::cout << "caught regex_error\n";
        std::cout << "unbalanced parenthesis : "
                  << (e.code() == std::regex_constants::error_paren) << '\n';
    }

    // A valid pattern, for contrast: it builds and matches normally.
    std::regex good(R"(\(closed\))");      // matches the literal text "(closed)"
    std::cout << "valid pattern on '(closed)' : "
              << std::regex_search(std::string("(closed)"), good) << '\n';
    return 0;
}
