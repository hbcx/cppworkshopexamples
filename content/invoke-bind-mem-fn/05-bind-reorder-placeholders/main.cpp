#include <functional>
#include <iostream>
#include <string>

std::string join3(const std::string& a, const std::string& b, const std::string& c) {
    return a + "-" + b + "-" + c;
}

int sub(int a, int b) { return a - b; }

int main() {
    using namespace std::placeholders;

    // A placeholder's NUMBER selects which call argument it maps to, not its
    // position inside the bind. _1 = first call argument, _2 = second, and so on.

    // Reverse the argument order: _2 goes first, _1 second.
    auto swapped = std::bind(sub, _2, _1);
    std::cout << "sub(10, 3)     = " << sub(10, 3) << "\n";        // 7
    std::cout << "swapped(10, 3) = " << swapped(10, 3) << "\n";    // 3 - 10 = -7

    // Repeat a placeholder: the same call argument is used more than once.
    auto twice = std::bind(sub, _1, _1);
    std::cout << "twice(9)       = " << twice(9) << "\n";          // 0

    // Rearrange three arguments and mix a fixed one in: a=_3, b="MID", c=_1.
    auto reordered = std::bind(join3, _3, "MID", _1);
    std::cout << "reordered      = " << reordered("A", "B", "C") << "\n";  // C-MID-A
    // "B" is the second call argument and no placeholder names it, so it is
    // simply ignored -- see the anti-pattern on silently dropped arguments.
    return 0;
}
