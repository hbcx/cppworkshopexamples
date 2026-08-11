#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

struct Widget {
    std::string tag() const { return "w"; }
};

int main() {
    // GOOD: declval only in an UNEVALUATED context (decltype here). No code runs.
    using TagType = decltype(std::declval<Widget>().tag());   // std::string
    static_assert(std::is_same<TagType, std::string>::value, "tag() returns std::string");

    // BAD (described, not compiled): std::declval has NO definition -- it exists
    // only to be named inside decltype and friends. Using it where the expression
    // is actually EVALUATED compiles but does NOT link:
    //   Widget w = std::declval<Widget>();        // LINK ERROR: undefined reference
    //   auto t   = std::declval<Widget>().tag();  // LINK ERROR too
    // To get a real object, construct one -- declval is purely a type-level tool.

    Widget real;
    std::cout << "real tag: " << real.tag() << "  (declval was used only for the TYPE above)\n";
    return 0;
}
