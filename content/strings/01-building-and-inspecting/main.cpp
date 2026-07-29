#include <iostream>
#include <string>

int main() {
    std::string fromLiteral = "hello";
    std::string filled(3, 'x');                 // "xxx" -- count + fill char
    std::string slice(fromLiteral, 1, 3);       // "ell" -- from pos 1, length 3

    std::cout << "fromLiteral = " << fromLiteral << " (size " << fromLiteral.size() << ")\n";
    std::cout << "filled      = " << filled << '\n';
    std::cout << "slice       = " << slice << '\n';
    std::cout << "empty?      " << (std::string().empty() ? "yes" : "no") << '\n';

    // Two accessors, two contracts. at() checks and throws; operator[] does not.
    std::cout << "at(0)       = " << fromLiteral.at(0) << '\n';
    std::cout << "front/back  = " << fromLiteral.front() << ' ' << fromLiteral.back() << '\n';
    try {
        (void)fromLiteral.at(99);   // (void) silences the [[nodiscard]] on at()
    } catch (const std::out_of_range&) {
        std::cout << "at(99)      threw out_of_range\n";
    }

    // A string is a container of char: range-for and the algorithms apply.
    int letters = 0;
    for (char c : fromLiteral)
        if (c != 'l') ++letters;
    std::cout << "non-l chars = " << letters << '\n';
    return 0;
}
