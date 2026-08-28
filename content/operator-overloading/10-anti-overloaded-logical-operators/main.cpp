#include <iostream>

// Anti-pattern: overloading && so a type can be combined with "and". It compiles
// and looks natural, but it quietly throws away short-circuit evaluation, because
// a user-defined operator&& is just a function -- both operands are evaluated
// before it is called.

int evalCount = 0;   // counts how many times the right-hand side actually runs

struct Cond {
    bool value;
    explicit Cond(bool v) : value(v) {}
};

// BAD: as a function, this receives both arguments already evaluated.
Cond operator&&(Cond a, Cond b) { return Cond(a.value && b.value); }

// A right-hand side with a visible side effect, in each form.
Cond checkRhs()      { ++evalCount; return Cond(true); }
bool checkRhsBool()  { ++evalCount; return true; }

int main() {
    // Overloaded &&: the left side is false, yet checkRhs() STILL runs, because it
    // is an argument to operator&&. Short-circuit is gone.
    evalCount = 0;
    Cond bad = Cond(false) && checkRhs();
    std::cout << "overloaded &&: result=" << bad.value
              << ", rhs evaluated " << evalCount << " time(s)\n";   // 0, 1

    // THE FIX: no operator&&. Expose the truth value (here a plain bool) and let
    // the BUILT-IN && combine them -- it short-circuits, so the false left side
    // skips the right entirely.
    evalCount = 0;
    bool good = false && checkRhsBool();
    std::cout << "built-in   &&: result=" << good
              << ", rhs evaluated " << evalCount << " time(s)\n";   // 0, 0
    return 0;
}
