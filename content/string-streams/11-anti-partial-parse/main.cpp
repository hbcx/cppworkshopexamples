#include <iostream>
#include <sstream>
#include <string>

// A complete parse: the extraction worked AND nothing but trailing whitespace
// is left. Skip whitespace, then require end of stream.
static bool parseFullInt(const std::string& text, int& out) {
    std::istringstream in(text);
    in >> out;
    return !in.fail() && (in >> std::ws).eof();
}

int main() {
    // Anti-pattern: treating a successful >> as a valid, COMPLETE parse. >> reads
    // as far as it can and stops; "42abc" extracts 42 and leaves "abc", so the
    // read "succeeds" while the input was not really a number.
    std::istringstream in("42abc");
    int n = 0;
    in >> n;
    std::cout << "read succeeded: " << static_cast<bool>(in)
              << " value: " << n << '\n';                                   // 1, 42
    std::cout << "but junk remains (not at end): " << !in.eof() << '\n';    // 1

    // Fix: require the WHOLE string to be consumed.
    int value = 0;
    std::cout << "parse '42':    " << parseFullInt("42", value)
              << " -> " << value << '\n';                                   // 1 -> 42
    std::cout << "parse '42abc': " << parseFullInt("42abc", value) << '\n'; // 0
    std::cout << "parse '  7  ': " << parseFullInt("  7  ", value)
              << " -> " << value << '\n';                                   // 1 -> 7
    return 0;
}
