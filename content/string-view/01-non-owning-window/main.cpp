#include <iostream>
#include <string>
#include <string_view>

// One signature serves std::string, a literal, and a char buffer -- no
// allocation, no conversion, at any call site.
static std::size_t countChar(std::string_view text, char c) {
    std::size_t n = 0;
    for (char ch : text)
        if (ch == c) ++n;
    return n;
}

int main() {
    std::string owned = "mississippi";
    std::cout << "'s' in string  = " << countChar(owned, 's') << '\n';
    std::cout << "'s' in literal = " << countChar("classless", 's') << '\n';

    // substr on a view is free: another window into the SAME buffer, no copy.
    std::string_view whole = owned;
    std::string_view tail = whole.substr(4);       // "issippi", points into owned
    std::cout << "tail = " << tail << " (size " << tail.size() << ")\n";

    // remove_prefix/suffix shrink the window by moving its endpoints.
    std::string_view middle = whole;
    middle.remove_prefix(2);
    middle.remove_suffix(2);
    std::cout << "middle = " << middle << '\n';     // "ssissip"

    std::cout << "view owns nothing; sizeof(string_view) = "
              << sizeof(std::string_view) << " bytes\n";
    return 0;
}
