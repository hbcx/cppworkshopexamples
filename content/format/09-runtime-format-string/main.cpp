#include <format>
#include <iostream>
#include <string>

int main() {
    // WRONG (won't compile): std::format needs the format string to be a
    // compile-time constant so it can check it against the argument types. A
    // plain std::string is not a constant, so this does not build:
    //   std::string fmt = "{} items";
    //   std::string s = std::format(fmt, 42);      // error: not a constant

    // RIGHT: for a runtime format string, use std::vformat with make_format_args.
    // It takes a string_view and a type-erased pack and skips the compile check.
    std::string fmt = "{} items";
    int count = 42;
    std::string s = std::vformat(fmt, std::make_format_args(count));
    std::cout << s << '\n';                           // 42 items

    // If the format string is a literal, keep std::format -- you WANT the check.
    std::cout << std::format("{} items (checked)\n", count);
    return 0;
}
