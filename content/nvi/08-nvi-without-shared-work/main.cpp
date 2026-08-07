#include <iostream>

// BAD: the non-virtual wrapper does NOTHING but forward to the virtual step.
// NVI earns its keep only when the wrapper holds shared work (a check, a lock,
// a log); an empty wrapper is just indirection.
struct BadShape {
    virtual ~BadShape() = default;
    void area() const { computeArea(); }        // adds nothing
private:
    virtual void computeArea() const = 0;
};
struct BadCircle : BadShape {
    void computeArea() const override { std::cout << "circle area\n"; }
};

// GOOD: with no shared work to enforce, a plain public virtual is simpler and
// reads as exactly what it is -- one overridable operation.
struct GoodShape {
    virtual ~GoodShape() = default;
    virtual void area() const = 0;
};
struct GoodCircle : GoodShape {
    void area() const override { std::cout << "circle area\n"; }
};

int main() {
    BadCircle{}.area();
    GoodCircle{}.area();

    // Use NVI when the base has real cross-cutting work to run around the
    // virtual step. If the wrapper would be empty, prefer a plain public
    // virtual -- the extra layer only hides the call.
    return 0;
}
