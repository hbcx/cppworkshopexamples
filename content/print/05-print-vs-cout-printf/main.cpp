#include <print>
#include <string>

int main() {
    std::string name = "Ada";
    int count = 3;

    // Why prefer std::print:
    //   std::cout: works, but a verbose << chain, and formatting needs manipulators
    //     std::cout << "user " << name << " has " << count << " items\n";
    //   printf: terse, but NOT type-safe -- a wrong specifier is undefined behaviour
    //     std::printf("user %s has %d items\n", name.c_str(), count);  // needs c_str()
    //   std::print: type-safe, compile-time-checked format, takes std::string directly.
    std::println("user {} has {} items", name, count);

    // The format string is checked at COMPILE time: a placeholder with no
    // matching argument, or a spec that does not fit, is a compile error, not a
    // run-time crash like a mismatched printf specifier.
    std::println("{} squared is {}", count, count * count);
    return 0;
}
