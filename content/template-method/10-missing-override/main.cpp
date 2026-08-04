// Anti-pattern: a step that does not actually override (missing override).
//
// A subclass step whose signature differs from the base virtual -- a missing
// const, a different parameter type -- does NOT override it. It compiles as a
// brand-new function, and the skeleton, calling through the base, runs the BASE
// version instead of the one you wrote. Here CustomBad::line() drops the const
// the base declared, so it never overrides, and render() prints the base line
// for a CustomBad. A good compiler warns about this (-Woverloaded-virtual), and
// writing `override` turns it into a hard error -- so this file sets
// `werror: false` to let the deliberately-warning bad case build and run and
// show the base version taking over. The good version uses `override` and the
// correct signature, and needs neither.

#include <iostream>
#include <string>

// ---------- BAD: signature mismatch -> not an override -> base version runs ----------
struct FormatterBad {
    virtual ~FormatterBad() = default;
    void render() { std::cout << line() << "\n"; }        // skeleton calls line()
    virtual std::string line() const { return "base line"; } // note the const
};

struct CustomBad : FormatterBad {
    // Intended to override, but MISSING const -> a new, unrelated function that
    // HIDES the base virtual rather than overriding it. render() (calling
    // through FormatterBad) never reaches this one.
    std::string line() { return "custom line"; }
};

// ---------- GOOD: correct signature + override, so it really overrides ----------
struct FormatterGood {
    virtual ~FormatterGood() = default;
    void render() { std::cout << line() << "\n"; }
    virtual std::string line() const { return "base line"; }
};

struct CustomGood : FormatterGood {
    // `override` makes the compiler verify this truly overrides -- had the const
    // been missing here, this line would fail to compile instead of silently not
    // overriding.
    std::string line() const override { return "custom line"; }
};

int main() {
    std::cout << "--- bad (silently not overriding) ---\n";
    CustomBad{}.render();   // base line   <- the override that never was

    std::cout << "--- good (override verified by the compiler) ---\n";
    CustomGood{}.render();  // custom line
    return 0;
}
