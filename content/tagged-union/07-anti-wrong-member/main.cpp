#include <iostream>

struct Number {
    enum class Kind { Int, Double } tag;
    union {
        int    i;
        double d;
    };
};

int main() {
    Number n;
    n.tag = Number::Kind::Int;
    n.i = 65;

    // --- the trap (NOT executed: reading the inactive member is UB) ---
    // n is tagged Int, so n.i is the active member. Reading n.d instead --
    //   double wrong = n.d;
    // reads a double out of bytes that hold an int: undefined behaviour, and the
    // value differs by compiler and build. The tag exists so you never do this.

    // --- the fix: check the tag and read the member it says is active ---
    switch (n.tag) {
        case Number::Kind::Int:    std::cout << "int " << n.i << '\n';    break;   // int 65
        case Number::Kind::Double: std::cout << "double " << n.d << '\n'; break;
    }
    std::cout << "always read the member the tag says is active\n";
    return 0;
}
