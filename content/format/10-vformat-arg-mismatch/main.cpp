#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    int one = 1;

    // A runtime format string skips the compile-time check, so a mismatch between
    // placeholders and arguments is only caught when the code RUNS: std::vformat
    // throws std::format_error rather than failing to build.
    std::string tooMany = "{} of {}";        // two placeholders...
    try {
        std::string s = std::vformat(tooMany, std::make_format_args(one));  // ...one argument
        std::cout << s << '\n';              // not reached
    } catch (const std::format_error&) {
        std::cout << "vformat threw format_error: not enough arguments\n";
    }

    // RIGHT: keep the runtime string and its arguments in sync, and validate any
    // externally-supplied format string before trusting it.
    std::string ok = "{} of {}";
    int two = 2;
    std::cout << std::vformat(ok, std::make_format_args(one, two)) << '\n';  // 1 of 2
    return 0;
}
