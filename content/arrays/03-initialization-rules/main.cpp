#include <array>
#include <iostream>
#include <string>

template <typename Array>
static void print(const Array& a, const char* label) {
    std::cout << label;
    for (const auto& x : a) {
        std::cout << " [" << x << ']';
    }
    std::cout << '\n';
}

int main() {
    // Anti-pattern (shown, NOT run): no braces means no initialization. For a
    // trivial element type the values are INDETERMINATE -- reading one is
    // undefined behaviour, and it will often look like zeros in a debug build
    // and stop looking like zeros in release:
    //   std::array<int, 3> garbage;
    //   std::cout << garbage[0];      // UB
    // Every other container default-constructs into a defined state; array does
    // not, because it is an aggregate and follows the aggregate rules.

    // Braces value-initialize: every element is zero.
    std::array<int, 3> zeroed{};
    print(zeroed, "std::array<int,3> a{}      ");

    // Naming SOME elements value-initializes the rest, so the tail is zeros
    // rather than garbage -- the same rule, working in your favour.
    std::array<int, 3> partial{7};
    print(partial, "std::array<int,3> a{7}     ");

    std::array<int, 3> full{1, 2, 3};
    print(full, "std::array<int,3> a{1,2,3} ");

    // A class type default-constructs regardless of the braces: only trivial
    // types can hold garbage here. Both of these hold two empty strings.
    std::array<std::string, 2> names;
    print(names, "std::array<string,2> a     ");
    std::cout << "  (two empty strings -- std::string's constructor ran)\n";

    // fill() is the way to set every element after the fact.
    std::array<int, 3> filled;
    filled.fill(9);          // defined: fill WRITES, it does not read
    print(filled, "after fill(9)              ");
    return 0;
}
