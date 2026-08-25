#include <print>

int main() {
    // std::print writes formatted text; std::println does the same and adds a
    // newline. The format string uses {} placeholders filled by the arguments.
    std::print("no newline here; ");
    std::print("still the same line\n");

    std::println("hello, {}!", "world");
    std::println("{} + {} = {}", 2, 3, 2 + 3);

    // println with no arguments just writes a blank line.
    std::println();
    std::println("after a blank line");
    return 0;
}
