// Anti-pattern: steps coupled through shared protected state and call order.
//
// When steps communicate by reading and writing shared protected members and
// rely on the skeleton calling them in a particular order, a subclass that
// overrides one step can silently break an invariant another step assumed. Here
// prepare() sets a protected prepared_ that serve() reads; a subclass overrides
// prepare() but forgets to set prepared_, so serve() reads the sentinel and
// produces a defined-but-wrong result -- no crash, no warning. The fix is to
// pass data between steps explicitly (return it, hand it on) instead of through
// hidden shared state, so a step that skips its job cannot be missed.

#include <iostream>

// ---------- BAD: steps share a protected member and a hidden ordering ----------
class MealBad {
public:
    virtual ~MealBad() = default;
    void make() { prepare(); serve(); }         // order matters, invisibly
protected:
    int prepared_ = -1;                          // set by prepare(), read by serve()
    virtual void prepare() { prepared_ = 42; }   // the contract serve() depends on
    void serve() { std::cout << "serving " << prepared_ << "\n"; }
};

class RushedMeal : public MealBad {
protected:
    void prepare() override {                    // overrides prepare()...
        std::cout << "rushing\n";                // ...and forgets to set prepared_
    }
};

// ---------- GOOD: data flows through the interface, not a shared member ----------
class MealGood {
public:
    virtual ~MealGood() = default;
    void make() { serve(prepare()); }            // prepare's result is handed to serve
protected:
    virtual int prepare() = 0;                    // MUST return the value serve needs
    void serve(int prepared) { std::cout << "serving " << prepared << "\n"; }
};

class QuickMeal : public MealGood {
protected:
    int prepare() override { std::cout << "quick\n"; return 7; } // cannot skip the result
};

int main() {
    std::cout << "--- bad (override forgot the shared member) ---\n";
    RushedMeal{}.make();   // serving -1  <- sentinel leaked through

    std::cout << "--- good (data passed explicitly) ---\n";
    QuickMeal{}.make();    // serving 7
    return 0;
}
