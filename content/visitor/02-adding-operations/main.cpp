// Adding operations without touching the elements.
//
// This is Visitor's payoff. The shape classes below are exactly as in the basics
// example -- unchanged. To add a SECOND operation (perimeter) on top of the first
// (area), we write a second visitor class and change no shape. Each operation is
// an object; adding one is adding a class, never editing Circle or Square. The
// mirror-image cost -- adding a shape TYPE would force every visitor to gain a
// visit method -- is the expression problem, discussed in the notes.

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
    void accept(ShapeVisitor& v) const override { v.visit(*this); }
};
class Square : public Shape {
    double side_;
public:
    explicit Square(double s) : side_(s) {}
    double side() const { return side_; }
    void accept(ShapeVisitor& v) const override { v.visit(*this); }
};

// Operation 1: area.
class AreaVisitor : public ShapeVisitor {
public:
    void visit(const Circle& c) override {
        std::cout << "area: " << 3.14159 * c.radius() * c.radius() << "\n";
    }
    void visit(const Square& s) override {
        std::cout << "area: " << s.side() * s.side() << "\n";
    }
};

// Operation 2: perimeter -- added later, with NO change to Circle or Square.
class PerimeterVisitor : public ShapeVisitor {
public:
    void visit(const Circle& c) override {
        std::cout << "perimeter: " << 2 * 3.14159 * c.radius() << "\n";
    }
    void visit(const Square& s) override {
        std::cout << "perimeter: " << 4 * s.side() << "\n";
    }
};

int main() {
    Circle circle(2.0);
    Square square(3.0);
    const Shape* shapes[] = { &circle, &square };

    AreaVisitor area;
    PerimeterVisitor perimeter;
    for (const Shape* s : shapes) {
        s->accept(area);      // same elements...
        s->accept(perimeter); // ...two different operations
    }
    return 0;
}
