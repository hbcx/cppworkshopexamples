#include <iostream>
#include <compare>

// Anti-pattern: treating the <=> result like a bool or an int. It is an ordering
// object. The wrong forms below do not compile at all; the right ones compare it
// to the literal 0 (or use the named helpers).

int main() {
    int a = 3, b = 5;

    // WRONG (each is a compile error, shown here only as a comment):
    //   if (a <=> b) { ... }            // ordering does not convert to bool
    //   if ((a <=> b) == 1) { ... }     // compares only to the literal 0, not 1
    //   if ((a <=> b) < (a <=> b)) {}   // two results are not comparable together

    // RIGHT: compare the result to 0.
    std::cout << "a < b?  " << ((a <=> b) < 0)  << "\n";   // 1
    std::cout << "a == b? " << ((a <=> b) == 0) << "\n";   // 0
    std::cout << "a > b?  " << ((a <=> b) > 0)  << "\n";   // 0

    // RIGHT: the named helpers say the same thing more readably.
    std::cout << "is_lt?  " << std::is_lt(a <=> b) << "\n";   // 1
    std::cout << "is_eq?  " << std::is_eq(a <=> b) << "\n";   // 0

    // And for a single yes/no test, the plain operator is clearest of all:
    std::cout << "a < b (direct)? " << (a < b) << "\n";      // 1
    return 0;
}
