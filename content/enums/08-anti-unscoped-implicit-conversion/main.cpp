#include <iostream>

// Anti-pattern: relying on a plain enum, whose silent conversion to int lets
// nonsense compile. An unscoped enum decays to int with no warning, so it mixes
// freely with plain integers -- assigned to one, compared to one, used in
// arithmetic -- even when that makes no sense. (Comparing two DIFFERENT enums is
// now caught by -Wenum-compare, but the enum-to-int leak below still passes
// silently.)

enum Color { Red, Green, Blue };

int use_as_int(int code) { return code; }   // expects a plain int...

int main() {
    Color c = Green;    // 1

    // Silently converts to int -- no cast, no warning:
    int n = c;
    std::cout << "Color assigned to int: " << n << "\n";         // 1

    // Passed where an int is expected -- also silent:
    std::cout << "passed as int: " << use_as_int(c) << "\n";     // 1

    // Compared against a bare int literal -- compiles, and is a bug waiting to
    // happen (the 1 is really Green, but nothing in the code says so):
    std::cout << "c == 1? " << (c == 1) << "\n";                 // 1

    // Mixed into arithmetic with an int, producing a plain int:
    int shifted = c + 10;
    std::cout << "c + 10 = " << shifted << "\n";                 // 11

    // THE FIX: enum class. With a scoped enum, int n = c; c == 1; and c + 10 are
    // all compile errors -- the enum no longer silently becomes an int, so these
    // meaningless mixings stop compiling.
    return 0;
}
