#include <print>

int main() {
    // Anti-pattern: adding '\n' to a std::println. println ALREADY appends a
    // newline, so "line\n" produces the line PLUS a blank line after it -- the
    // classic double-spacing bug when converting from printf or cout.
    std::println("intended single line\n");   // WRONG: prints an extra blank line
    std::println("next line");

    std::println("---");

    // Fix: use println WITHOUT a trailing '\n' (it adds one), or use print WITH
    // an explicit '\n' if you want the control. Do not do both.
    std::println("single line, no extra newline");    // right
    std::print("also fine with explicit newline\n");  // right (print adds none)
    return 0;
}
