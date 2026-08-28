#include <iostream>

// Anti-pattern: overloading operator^ to mean "raise to a power". C++ has no power
// operator, and ^ is bitwise XOR, so this both misleads the reader and -- worse --
// inherits XOR's precedence, which is LOWER than +. The parse is not what the code
// looks like.

struct Num {
    long v;
};

Num operator+(Num a, Num b) { return Num{a.v + b.v}; }

// BAD: ^ now means power. Syntactically tempting, semantically a trap.
Num operator^(Num a, Num b) {
    long result = 1;
    for (long i = 0; i < b.v; ++i) result *= a.v;
    return Num{result};
}

// FIX: a named function. Intent and grouping are both unambiguous.
Num power(Num base, Num exp) { return base ^ exp; }   // reuse the math, hide the operator

int main() {
    // Looks like "(2 to the 1) + 2" = 4. Actually parses as "2 to the (1 + 2)" = 8,
    // because + binds tighter than ^. Nothing at the call site reveals this.
    Num surprise = Num{2} ^ Num{1} + Num{2};
    std::cout << "2 ^ 1 + 2 gave " << surprise.v << " (reader expected 4)\n";  // 8

    // The named version leaves no room for doubt:
    Num clear = power(Num{2}, Num{1});
    std::cout << "power(2, 1) + 2 = " << (clear.v + 2) << "\n";                 // 4
    return 0;
}
