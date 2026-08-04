// Visitor basics: double dispatch.
//
// An operation on a family of element types is packaged as a visitor object.
// Each element has accept(visitor), which calls visitor.visit(*this) -- and
// because *this inside accept has the element's concrete type, the right visit
// overload is chosen. The operation is thus selected by TWO dynamic types at
// once: the element's (via the virtual accept) and the visitor's (via the
// virtual visit). That is double dispatch, and it is what lets one visitor
// handle every element type without the caller testing types by hand.

#include <iostream>

class Circle;
class Square;

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
    void accept(ShapeVisitor& v) const override { v.visit(*this); } // *this is Circle here
};

class Square : public Shape {
    double side_;
public:
    explicit Square(double s) : side_(s) {}
    double side() const { return side_; }
    void accept(ShapeVisitor& v) const override { v.visit(*this); } // *this is Square here
};

// A concrete visitor: one visit per element type.
class AreaPrinter : public ShapeVisitor {
public:
    void visit(const Circle& c) override {
        std::cout << "circle area = " << 3.14159 * c.radius() * c.radius() << "\n";
    }
    void visit(const Square& s) override {
        std::cout << "square area = " << s.side() * s.side() << "\n";
    }
};

int main() {
    Circle circle(2.0);
    Square square(3.0);
    const Shape* shapes[] = { &circle, &square };

    AreaPrinter printer;
    for (const Shape* s : shapes)
        s->accept(printer); // accept picks the element type, visit picks the operation
    return 0;
}
