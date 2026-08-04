// Anti-pattern: a dynamic_cast type-switch instead of a visitor.
//
// This is the design Visitor replaces. An operation over a class hierarchy is
// written as a cascade of dynamic_cast tests: if it is a Circle do this, else if
// a Square do that. It works, but the type list is duplicated in every such
// operation, a forgotten type falls through to a silent default, and adding a
// type means hunting down every cascade. Double dispatch does the type selection
// once, in accept, and the compiler makes each visitor list every type.

#include <iostream>

class Circle;
class Square;

// The visitor form (the fix), for comparison.
class ShapeVisitor {
public:
    virtual ~ShapeVisitor() = default;
    virtual void visit(const Circle& c) = 0;
    virtual void visit(const Square& s) = 0;
};
class Shape {
public:
    virtual ~Shape() = default;
    virtual void accept(ShapeVisitor& v) const = 0;
};
class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double r) : radius_(r) {}
    double radius() const { return radius_; }
    void accept(ShapeVisitor& v) const override { v.visit(*this); }
};
class Square : public Shape {
    double side_;
public:
    explicit Square(double s) : side_(s) {}
    double side() const { return side_; }
    void accept(ShapeVisitor& v) const override { v.visit(*this); }
};

// ---- BAD: an operation that switches on the runtime type by hand ----
double areaBad(const Shape& s) {
    if (auto* c = dynamic_cast<const Circle*>(&s)) return 3.14159 * c->radius() * c->radius();
    if (auto* sq = dynamic_cast<const Square*>(&s)) return sq->side() * sq->side();
    return 0.0; // a forgotten type silently returns 0 -- and every operation repeats this cascade
}

// ---- GOOD: a visitor -- the type selection happens once, in accept ----
class AreaVisitor : public ShapeVisitor {
    double result_ = 0.0;
public:
    double result() const { return result_; }
    void visit(const Circle& c) override { result_ = 3.14159 * c.radius() * c.radius(); }
    void visit(const Square& s) override { result_ = s.side() * s.side(); }
};

int main() {
    Circle circle(2.0);
    Square square(3.0);
    const Shape* shapes[] = { &circle, &square };

    std::cout << "bad (dynamic_cast):\n";
    for (const Shape* s : shapes)
        std::cout << "  area = " << areaBad(*s) << "\n";

    std::cout << "good (visitor):\n";
    for (const Shape* s : shapes) {
        AreaVisitor v;
        s->accept(v);
        std::cout << "  area = " << v.result() << "\n";
    }
    return 0;
}
