#include <iostream>
#include <utility>

int main() {
    int delta = -3;
    unsigned threshold = 2;

    std::cout << std::boolalpha;

    // ANTI-PATTERN: comparing a signed value with an unsigned one. The signed -3 is
    // converted to a huge unsigned before the comparison, so "delta > threshold"
    // is TRUE even though -3 is not greater than 2. A guard like this silently does
    // the opposite of what it reads. The compiler warns -Wsign-compare -- exactly
    // the signal to heed; it is suppressed here only to run the bug and show it.
    bool buggy;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
    buggy = (delta > threshold);
#pragma GCC diagnostic pop
    std::cout << "delta > threshold  (built-in) = " << buggy << "  (wrong: -3 is not > 2)\n";

    // FIX: compare by value with std::cmp_greater (or keep both sides the same
    // signedness in the first place).
    bool fixed = std::cmp_greater(delta, threshold);
    std::cout << "cmp_greater(delta, threshold) = " << fixed << "  (correct)\n";

    // The other classic form is a reverse loop with an unsigned index:
    //   for (std::size_t i = n - 1; i >= 0; --i) ...   // i >= 0 is ALWAYS true -> infinite
    // shown here as a comment because it never terminates.
    return 0;
}
