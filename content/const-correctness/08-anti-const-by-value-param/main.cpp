#include <iostream>
#include <cstddef>

// Anti-pattern: a top-level const on a BY-VALUE parameter. In a function's type
// that const is ignored -- void f(int) and void f(const int) are the SAME
// function, not an overload pair. The const only affects the function body (its
// own local copy), never the caller, so writing it in a header promises
// something it does not deliver and cannot be overloaded on.

// These two would be the SAME function -- declaring both is a redefinition, not
// an overload. (Shown as a comment because it would not compile.)
//     int f(int x);
//     int f(const int x);   // redeclares the same f, not a new overload

// The const here only means "I will not change my own copy inside the body".
// The caller's argument is copied anyway, so this changes nothing for them.
int doubled(const int x) {     // const is local-only; the type is doubled(int)
    // x = x * 2;              // would be an error, but only inside this body
    return x * 2;
}

// A pointer or reference parameter is different: there the const is LOW-level
// and real -- it protects what the caller passed.
std::size_t lengthOf(const char* s) {   // cannot write through s: meaningful
    std::size_t n = 0;
    while (s[n] != '\0') ++n;
    return n;
}

int main() {
    int v = 21;
    std::cout << "doubled(21) = " << doubled(v) << "\n";        // 42
    std::cout << "v unchanged = " << v << "\n";                 // 21
    std::cout << "lengthOf    = " << lengthOf("hello") << "\n"; // 5
    return 0;
}
