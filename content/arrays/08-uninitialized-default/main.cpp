#include <array>
#include <iostream>
#include <vector>

// The trap survives inside a struct, which is where it usually hides: a
// defaulted constructor does NOT initialize a member array of a trivial type.
struct Config {
    std::array<int, 4> limits;      // indeterminate after `Config c;`
};

// The same struct, fixed once for every constructor it will ever have.
struct SafeConfig {
    std::array<int, 4> limits{};    // default member initializer: always zeroed
};

template <typename Array>
static void print(const Array& a, const char* label) {
    std::cout << label;
    for (const auto& x : a) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';
}

int main() {
    // Anti-pattern (shown, NOT run): no braces, so the elements are
    // indeterminate. Reading one is undefined behaviour -- and on a fresh stack
    // page it usually prints zeros, passes the test, and fails in release:
    //   std::array<int, 4> a;
    //   std::cout << a[0];         // UB
    //   Config c;
    //   std::cout << c.limits[0];  // UB, same reason, better hidden
    //
    // Note what does NOT save you: this is the only container that behaves this
    // way. A vector of the same length is defined and empty, no braces needed.
    std::vector<int> definedByDefault(4);
    print(definedByDefault, "std::vector<int> v(4):        ");

    // The fix is one character.
    std::array<int, 4> zeroed{};
    print(zeroed, "std::array<int,4> a{}:        ");

    // In a struct, braces on the object do the same job...
    SafeConfig safe;
    print(safe.limits, "SafeConfig s (member has {}): ");

    Config explicitlyZeroed{};
    print(explicitlyZeroed.limits, "Config c{} (braces at use):   ");

    // ...but the default member initializer is the one that cannot be forgotten
    // at the call site, which is why it is the better of the two.
    std::cout << "\nRule: brace it at the declaration, every time. It costs nothing\n"
                 "when you overwrite the elements anyway -- the compiler folds it away.\n";
    return 0;
}
