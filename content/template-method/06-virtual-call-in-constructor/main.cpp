// Anti-pattern: running the template method from the constructor.
//
// It is tempting to call the skeleton from the base constructor so an object is
// "ready" once built. But during the base constructor the object is still only a
// Base -- the derived part does not exist yet and the vtable points at Base --
// so a virtual step called from there runs the BASE version, never the override,
// even for a derived instance. The result is a silent wrong result (shown and
// run below). If the step were pure virtual, the call is undefined behaviour and
// typically aborts with "pure virtual method called" (described, not run). The
// fix is to run the skeleton AFTER construction, not inside it.

#include <iostream>

// ---------- BAD: skeleton called from the constructor ----------
class GreeterBad {
public:
    GreeterBad() {
        std::cout << "ctor calls step -> ";
        step(); // during GreeterBad's ctor, this is GreeterBad::step, not the override
    }
    virtual ~GreeterBad() = default;
    void run() { std::cout << "run  calls step -> "; step(); }
protected:
    virtual void step() { std::cout << "base step\n"; }
};

class FrenchGreeterBad : public GreeterBad {
protected:
    void step() override { std::cout << "bonjour\n"; }
};

// If step() above were `virtual void step() = 0;` (pure), the constructor's
// call would be UNDEFINED BEHAVIOUR -- there is no base implementation to run --
// and most implementations abort at runtime with "pure virtual method called".
// That is why this file keeps step() non-pure and never calls a pure virtual
// from a constructor.

// ---------- GOOD: construct fully, then run the skeleton ----------
class GreeterGood {
public:
    virtual ~GreeterGood() = default;
    void run() { std::cout << "run  calls step -> "; step(); } // called after construction
protected:
    virtual void step() = 0;
};

class FrenchGreeterGood : public GreeterGood {
protected:
    void step() override { std::cout << "bonjour\n"; }
};

int main() {
    std::cout << "--- bad ---\n";
    FrenchGreeterBad bad;   // ctor runs step() -> prints "base step" (WRONG)
    bad.run();              // after construction -> prints "bonjour" (right)

    std::cout << "--- good ---\n";
    FrenchGreeterGood good; // ctor does nothing risky
    good.run();             // -> "bonjour"
    return 0;
}
