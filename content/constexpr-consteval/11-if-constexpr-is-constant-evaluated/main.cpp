#include <iostream>
#include <type_traits>

// BAD: std::is_constant_evaluated() inside `if constexpr` is ALWAYS true. The
// condition of an if constexpr is itself a constant expression, evaluated in a
// constant-evaluated context, so the function reports true -- even at run time --
// and the run-time branch is dead code that never runs.
constexpr int markBad(int x) {
    if constexpr (std::is_constant_evaluated()) {   // ALWAYS true -- the bug
        return x + 1000;
    } else {
        return x;                                    // never reached
    }
}

// GOOD: use a PLAIN if. Its condition is evaluated per call -- true during
// constant evaluation, false at run time -- which is the whole point.
constexpr int markGood(int x) {
    if (std::is_constant_evaluated()) {
        return x + 1000;
    } else {
        return x;
    }
}

int main() {
    int r = 5;
    std::cout << "markBad  at run time: " << markBad(r)  << "  <- 1005, WRONG (dead branch)\n";
    std::cout << "markGood at run time: " << markGood(r) << "  <- 5, correct\n";

    constexpr int cb = markBad(5);
    constexpr int cg = markGood(5);
    std::cout << "markBad  at compile time: " << cb << "\n";   // 1005
    std::cout << "markGood at compile time: " << cg << "\n";   // 1005
    return 0;
}
