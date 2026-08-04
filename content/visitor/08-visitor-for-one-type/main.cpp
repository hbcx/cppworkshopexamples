// Anti-pattern: a visitor for a single type and a single operation.
//
// Visitor pays off when several operations range over several element types. For
// one element type with one operation, the whole apparatus -- a visitor base, an
// accept method, a concrete visitor -- is elaborate machinery around what a plain
// virtual method does directly. Below, computing a circle's area is wrapped in a
// visitor for no benefit; a member function is shorter and clearer.

#include <iostream>

// ---- BAD: full visitor scaffolding for one shape and one operation ----
class OnlyCircle;
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(const OnlyCircle&) = 0;
};
class Shape {
public:
    virtual ~Shape() = default;
    virtual void accept(Visitor&) const = 0;
};
class OnlyCircle : public Shape {
    double radius_;
public:
    explicit OnlyCircle(double r) : radius_(r) {}
    double radius() const { return radius_; }
    void accept(Visitor& v) const override { v.visit(*this); }
};
class AreaVisitor : public Visitor {
public:
    void visit(const OnlyCircle& c) override {
        std::cout << "area = " << 3.14159 * c.radius() * c.radius() << "\n";
    }
};

// ---- GOOD: just a method ----
class CircleGood {
    double radius_;
public:
    explicit CircleGood(double r) : radius_(r) {}
    double area() const { return 3.14159 * radius_ * radius_; }
};

int main() {
    OnlyCircle badCircle(2.0);
    AreaVisitor v;
    std::cout << "bad (visitor): ";
    badCircle.accept(v);

    CircleGood goodCircle(2.0);
    std::cout << "good (method): area = " << goodCircle.area() << "\n";
    return 0;
}
