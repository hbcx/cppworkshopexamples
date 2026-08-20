#include <format>
#include <iostream>
#include <string>

int main() {
    // The format string is checked AT COMPILE TIME against the argument types.
    // A literal with a bad spec would not compile -- for example
    //   std::format("{:d}", "text");   // error: a string has no 'd' presentation
    // which turns a whole class of printf-style runtime bugs into build errors.
    std::cout << std::format("checked: {} and {}\n", 1, "ok");

    // When the format string is only known at RUNTIME (from config, a translation
    // table, user input) the compile-time check cannot run. Use std::vformat: it
    // takes a string_view and a type-erased argument pack.
    std::string fmt = "runtime: {} and {}\n";     // value decided at run time
    int a = 10;
    std::string b = "twenty";
    std::cout << std::vformat(fmt, std::make_format_args(a, b));
    return 0;
}
