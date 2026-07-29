#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

static std::string trim(const std::string& s) {
    const char* blanks = " \t\n\r";
    std::string::size_type first = s.find_first_not_of(blanks);
    if (first == std::string::npos) return "";   // all blank -> empty
    std::string::size_type last = s.find_last_not_of(blanks);
    return s.substr(first, last - first + 1);
}

static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](char c) {
        // Cast to unsigned char first: tolower on a negative char is UB.
        return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    });
    return s;
}

int main() {
    std::cout << "[" << trim("   padded text   ") << "]\n";   // [padded text]
    std::cout << "all-blank trims to empty? "
              << (trim("      ").empty() ? "yes" : "no") << '\n';

    std::cout << toLower("Mixed CASE Text") << '\n';          // mixed case text

    // The reason for the unsigned-char cast: a byte above 127 is negative as a
    // plain char. Folding it must not pass a negative value to tolower.
    std::string highByte(1, static_cast<char>(200));
    std::cout << "high-byte fold did not crash, size = " << toLower(highByte).size() << '\n';
    return 0;
}
