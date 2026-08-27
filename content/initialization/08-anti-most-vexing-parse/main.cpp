#include <iostream>

// Anti-pattern: the "most vexing parse". When you write what looks like a
// variable with a constructor call, but the arguments could also be read as
// parameter declarations, C++ parses the whole line as a FUNCTION DECLARATION,
// not an object. The fix is brace initialization, which can never be a
// declaration.

struct Clock {};

struct Timer {
    Timer() = default;
    explicit Timer(Clock) {}
    int tick() const { return 1; }
};

int main() {
    // THE TRAP (shown as comments -- these do not do what they look like):
    //
    //     Timer t(Clock());
    //
    // That declares a FUNCTION named t returning Timer, taking a pointer to a
    // function returning Clock. It is not an object, so this fails:
    //     t.tick();      // error: 't' is a function, has no member 'tick'
    //
    // Even the simplest case bites:
    //     Timer u();     // declares a function u(), NOT a default Timer object
    //     u.tick();      // error

    // THE FIX: braces. {} can only be initialization, never a declaration.
    Timer t{Clock{}};
    std::cout << "t.tick() = " << t.tick() << "\n";

    Timer u{};                  // a default-constructed object, not a function
    std::cout << "u.tick() = " << u.tick() << "\n";

    // Extra parentheses around the argument also disambiguate, but braces are
    // the habit to build:
    Timer v((Clock()));
    std::cout << "v.tick() = " << v.tick() << "\n";
    return 0;
}
