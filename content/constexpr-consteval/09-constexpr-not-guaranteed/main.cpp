#include <iostream>
#include <type_traits>

// This constexpr function returns a mark of HOW it was evaluated, so we can prove
// when it actually ran at compile time versus run time. (+1000 only at compile time.)
constexpr int evalMark(int x) {
    return std::is_constant_evaluated() ? (x + 1000) : x;
}

int main() {
    // Forced to compile time (used to init a constexpr variable): +1000.
    constexpr int atCompile = evalMark(5);
    std::cout << "constexpr var: " << atCompile << "  <- compile time (1000 added)\n";

    // BAD assumption: "it is constexpr, so this must be compile time." It is NOT:
    // a run-time argument means the call runs at RUN TIME, no +1000.
    int r = 5;
    std::cout << "runtime arg:   " << evalMark(r) << "  <- run time\n";

    // Even with a LITERAL, a plain function argument is not a constant-expression
    // context, so the compiler is not required to fold it -- and here does not.
    std::cout << "plain call:    " << evalMark(5) << "  <- run time (not forced)\n";

    // The fix when you REQUIRE compile time: assign to a constexpr variable (as at
    // the top) or make the function consteval so a run-time call cannot compile.
    std::cout << "done\n";
}
