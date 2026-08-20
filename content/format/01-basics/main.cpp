#include <format>
#include <iostream>
#include <string>

int main() {
    // Each {} is a placeholder, filled by the next argument in order.
    std::string s = std::format("{} + {} = {}", 2, 3, 2 + 3);
    std::cout << s << '\n';                         // 2 + 3 = 5

    // std::format RETURNS a std::string; it does not print by itself.
    std::string name = "Ada";
    std::cout << std::format("Hello, {}!\n", name);

    // Positional indices reuse or reorder the arguments.
    std::cout << std::format("{0} {1} {0}\n", "ho", "hey");   // ho hey ho

    // A literal brace is written by doubling it.
    std::cout << std::format("a literal brace: {{ }}\n");     // a literal brace: { }
    return 0;
}
