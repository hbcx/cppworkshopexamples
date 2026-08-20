#include <iostream>
#include <regex>
#include <string>

// Put a backslash before every ECMAScript metacharacter, so `text` is matched
// LITERALLY instead of being read as a pattern.
static std::string escapeRegex(const std::string& text) {
    static const std::regex special(R"([.^$|()\[\]{}*+?\\])");
    return std::regex_replace(text, special, R"(\$&)");
}

int main() {
    std::cout << std::boolalpha;

    // WRONG: '.' in a pattern means "any character", so this also matches "aXc".
    std::regex naive("a.c");
    std::cout << "naive 'a.c' on 'aXc'          : "
              << std::regex_search(std::string("aXc"), naive) << '\n';   // true (unintended)

    // RIGHT: escape the dot to match a literal '.'.
    std::regex literal(R"(a\.c)");
    std::cout << "escaped 'a\\.c' on 'aXc'       : "
              << std::regex_search(std::string("aXc"), literal) << '\n'; // false
    std::cout << "escaped 'a\\.c' on 'a.c'       : "
              << std::regex_search(std::string("a.c"), literal) << '\n'; // true

    // WRONG in general: building a pattern straight from user input reads their
    // text AS regex -- a stray '(' throws, and '.*' becomes a wildcard (injection).
    // RIGHT: escape the input first, then it matches as plain text.
    std::regex fromUser(escapeRegex("a.c"));
    std::cout << "escaped user 'a.c' on 'aXc'   : "
              << std::regex_search(std::string("aXc"), fromUser) << '\n'; // false
    std::cout << "escaped user 'a.c' on 'a.c'   : "
              << std::regex_search(std::string("a.c"), fromUser) << '\n'; // true
    return 0;
}
