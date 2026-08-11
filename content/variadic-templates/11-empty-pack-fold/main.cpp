#include <iostream>

// GOOD: a BINARY fold carries an explicit identity value, so it is well-defined
// even for an empty pack.
template <class... Ts>
int sumSafe(Ts... args) {
    return (0 + ... + args);   // 0 is the identity; empty pack -> 0
}

// BAD (described, not compiled): a UNARY fold over + has NO identity, so it is
// ill-formed for an empty pack. Only &&, || and comma have a defined empty result.
//   template <class... Ts> int sumBad(Ts... args) { return (args + ...); }
//   sumBad();   // COMPILE ERROR: empty pack has no value for +

int main() {
    std::cout << "sumSafe(1,2,3) = " << sumSafe(1, 2, 3) << "\n";   // 6
    std::cout << "sumSafe()      = " << sumSafe() << "\n";          // 0 -- works because of the identity

    // The unary-fold version (args + ...) would fail to compile on the empty call.
    return 0;
}
