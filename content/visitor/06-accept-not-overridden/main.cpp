// Anti-pattern: an element that does not override accept.
//
// Double dispatch works only because each element overrides accept, so that
// inside accept *this has the element's concrete type and the right visit
// overload is chosen. If an element inherits a base-class accept instead of
// overriding it, *this is the BASE type there, and dispatch lands on the wrong
// (base) handler -- even when a correct visit overload for the element exists.
// Here Circle overrides accept and dispatches correctly; Wedge forgets to, and
// falls through to visitUnknown though visit(const Wedge&) is right there. The
// robust fix is to make accept pure virtual, so forgetting it will not compile.

#include <iostream>

class Circle;
class Wedge;

class ShapeVisitor {
public:
    virtual ~ShapeVisitor() = default;
    virtual void visit(const Circle&) = 0;
    virtual void visit(const Wedge&) = 0;      // the operation IS defined for Wedge
    virtual void visitUnknown(const char* name) = 0;
};

class Shape {
public:
    virtual ~Shape() = default;
    virtual const char* name() const = 0;
    // A convenience default -- the trap. A subclass that forgets to override
    // accept is silently dispatched here instead of to its own visit overload.
    virtual void accept(ShapeVisitor& v) const { v.visitUnknown(name()); }
};

class Circle : public Shape {
public:
    const char* name() const override { return "circle"; }
    void accept(ShapeVisitor& v) const override { v.visit(*this); } // correct
};

class Wedge : public Shape {
public:
    const char* name() const override { return "wedge"; }
    // BUG: no accept override -> Shape::accept runs -> visitUnknown, never visit(Wedge)
};

class Printer : public ShapeVisitor {
public:
    void visit(const Circle&) override { std::cout << "  handled circle\n"; }
    void visit(const Wedge&) override { std::cout << "  handled wedge\n"; }
    void visitUnknown(const char* name) override {
        std::cout << "  UNKNOWN: " << name << " (accept not overridden)\n";
    }
};

int main() {
    Circle circle;
    Wedge wedge;
    const Shape* shapes[] = { &circle, &wedge };

    Printer printer;
    for (const Shape* s : shapes)
        s->accept(printer); // circle dispatches right; wedge falls through
    return 0;
}
