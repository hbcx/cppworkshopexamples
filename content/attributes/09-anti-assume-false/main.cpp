#include <iostream>

// ANTI-PATTERN (described, NOT run -- it is undefined behaviour):
//
//   int half(int x) {
//       [[assume(x % 2 == 0)]];   // PROMISE the compiler that x is even
//       return x / 2;
//   }
//   half(7);   // x is odd -> the promise is false -> undefined behaviour
//
// [[assume]] is a promise the compiler TRUSTS without checking. If the condition
// can ever be false, the program has undefined behaviour: the optimizer is allowed
// to delete code and take shortcuts that are only valid when the assumption holds.
// It is not a runtime check -- unlike assert, nothing catches a violation.

// FIX: assume ONLY what is genuinely guaranteed -- by an earlier check, the type,
// or the caller's contract. Here every value passed below is non-negative, so the
// assumption is always true and the compiler can use it safely.
long scaled(int x) {
    [[assume(x >= 0)]];
    return x * 4L;
}

int main() {
    for (int x : {0, 3, 10}) {          // all non-negative -> the assumption holds
        std::cout << "scaled(" << x << ") = " << scaled(x) << "\n";
    }
    return 0;
}
