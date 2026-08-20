#include <array>
#include <iostream>
#include <string_view>

// A compile-time table of keywords. std::string_view is a literal type, so this
// table lives in read-only data with no runtime construction and no allocation --
// std::string cannot do this: its constructor is not constexpr before C++20.
constexpr std::array<std::string_view, 4> keywords = {
    "if", "else", "for", "while"
};

constexpr bool isKeyword(std::string_view w) {
    for (std::string_view k : keywords)
        if (k == w) return true;
    return false;
}

// find() is constexpr on a view, so the compiler evaluates this.
constexpr std::size_t firstSpace(std::string_view s) {
    return s.find(' ');
}

int main() {
    // Checked entirely at compile time -- no code runs for these.
    static_assert(isKeyword("for"), "for is a keyword");
    static_assert(!isKeyword("forx"), "forx is not");
    static_assert(firstSpace("int main") == 3, "space at index 3");

    std::cout << std::boolalpha;
    std::cout << "isKeyword(\"while\") = " << isKeyword("while") << '\n';
    std::cout << "isKeyword(\"foo\")   = " << isKeyword("foo") << '\n';

    // The same constexpr function also works on a runtime view.
    std::string_view probe = "else";
    std::cout << "isKeyword(probe)    = " << isKeyword(probe) << '\n';
    return 0;
}
