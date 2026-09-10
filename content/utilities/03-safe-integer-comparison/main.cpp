#include <iostream>
#include <utility>

int main() {
    int s = -1;
    unsigned u = 1;

    std::cout << std::boolalpha;

    // The built-in comparison converts -1 to a huge unsigned value first, so this
    // is false -- almost never what you meant. (The compiler warns -Wsign-compare;
    // we suppress it locally just to run it and show the wrong result.)
    bool builtin;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
    builtin = (s < u);
#pragma GCC diagnostic pop
    std::cout << "-1 < 1u   (built-in)      = " << builtin << "\n";

    // std::cmp_less compares by mathematical value, ignoring the signedness mix.
    std::cout << "cmp_less(-1, 1u)          = " << std::cmp_less(s, u) << "\n";
    std::cout << "cmp_greater(-1, 1u)       = " << std::cmp_greater(s, u) << "\n";
    std::cout << "cmp_equal(-1, 1u)         = " << std::cmp_equal(s, u) << "\n";
    return 0;
}
