#include <cctype>
#include <iostream>
#include <string>

// Case-insensitive equality: fold both sides one char at a time. tolower takes
// an int that must be representable as unsigned char, hence the cast.
static bool iequals(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (std::string::size_type i = 0; i < a.size(); ++i) {
        unsigned char ca = static_cast<unsigned char>(a[i]);
        unsigned char cb = static_cast<unsigned char>(b[i]);
        if (std::tolower(ca) != std::tolower(cb)) return false;
    }
    return true;
}

int main() {
    std::string a = "apple";
    std::string b = "apple";
    std::string c = "Apple";

    std::cout << "a == b?          " << (a == b ? "yes" : "no") << '\n';   // yes
    std::cout << "a == c?          " << (a == c ? "yes" : "no") << '\n';   // no (case)
    std::cout << "iequals(a, c)?   " << (iequals(a, c) ? "yes" : "no") << '\n';   // yes

    // Lexicographic order is by BYTE value: 'A' (65) sorts before 'a' (97).
    std::cout << "\"Apple\" < \"apple\"? " << (c < a ? "yes" : "no") << '\n';   // yes

    // compare() gives the three-way sign, like strcmp.
    int sign = a.compare(c);
    std::cout << "a.compare(c) is " << (sign < 0 ? "negative" : sign > 0 ? "positive" : "zero") << '\n';

    // Compare against a literal with no temporary string built.
    std::cout << "a == \"apple\"?    " << (a == "apple" ? "yes" : "no") << '\n';
    return 0;
}
