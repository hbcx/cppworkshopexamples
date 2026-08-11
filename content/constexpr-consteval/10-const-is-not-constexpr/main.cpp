#include <array>
#include <iostream>

int runtimeSize() { return 8; }               // not constexpr
constexpr int constexprSize() { return 8; }   // constexpr

int main() {
    // BAD: `const` only means "cannot be changed", NOT "known at compile time".
    // A const initialized from a run-time value is a RUN-TIME constant and cannot
    // be used where the language needs a constant expression.
    const int badSize = runtimeSize();
    std::cout << "const from runtime = " << badSize << "\n";
    // std::array<int, badSize> a;   // COMPILE ERROR: badSize is not a constant
    //                               // expression, even though it is const.

    // GOOD: `constexpr` means "must be a compile-time constant", so it works in a
    // constant-expression context.
    constexpr int goodSize = constexprSize();
    std::array<int, goodSize> arr{};
    std::cout << "constexpr size = " << goodSize << ", array holds " << arr.size() << "\n";

    // Note: a const initialized from a LITERAL (const int n = 8;) IS a constant
    // expression -- the trap is a const initialized from a non-constant value,
    // which looks the same but is not usable as one.
    std::cout << "done\n";
}
