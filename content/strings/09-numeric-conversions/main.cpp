#include <iostream>
#include <string>

int main() {
    // Number -> text.
    std::string s = "port " + std::to_string(8080);
    std::cout << s << '\n';

    // Text -> number.
    int port = std::stoi("8080");
    std::cout << "parsed port + 1 = " << port + 1 << '\n';

    // stoi parses a PREFIX and reports where it stopped via pos.
    std::size_t pos = 0;
    int n = std::stoi("42abc", &pos);
    std::cout << "stoi(\"42abc\") = " << n << ", stopped at index " << pos
              << (pos != 5 ? " (trailing junk)" : "") << '\n';

    // base 16.
    std::cout << "hex ff = " << std::stoi("ff", nullptr, 16) << '\n';

    // The parsers THROW on input that does not start with a number.
    try {
        std::stoi("not a number");
    } catch (const std::invalid_argument&) {
        std::cout << "stoi(\"not a number\") threw invalid_argument\n";
    }

    // Round-trip a double without printing its (fixed-format) text.
    double d = std::stod(std::to_string(3.5));
    std::cout << "double round-trips to 3.5? " << (d == 3.5 ? "yes" : "no") << '\n';
    return 0;
}
