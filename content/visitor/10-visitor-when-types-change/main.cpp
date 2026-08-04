// Anti-pattern: choosing Visitor when the TYPES change often.
//
// Visitor makes adding OPERATIONS easy and adding TYPES hard: every new element
// type forces a new visit method in every visitor. So when the element types are
// what keep changing, Visitor fights you -- adding a shape edits every visitor
// class. The arrangement that fits a growing type set is plain virtual methods:
// each operation is a virtual on the base, and a new type is one class that
// implements them, touching no existing code. This file shows that arrangement;
// the notes contrast it with the Visitor arrangement (examples 01-02).

#include <iostream>

// Operations are virtual methods on the element. Adding a TYPE is one new class.
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
};

class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double r) : radius_(r) {}
    double area() const override { return 3.14159 * radius_ * radius_; }
    double perimeter() const override { return 2 * 3.14159 * radius_; }
};
class Square : public Shape {
    double side_;
public:
    explicit Square(double s) : side_(s) {}
    double area() const override { return side_ * side_; }
    double perimeter() const override { return 4 * side_; }
};

// Adding Triangle touches NO existing class -- just this new one. That is the
// easy direction here; with Visitor it would edit every visitor instead.
class Triangle : public Shape {
    double base_, height_, side_;
public:
    Triangle(double b, double h, double s) : base_(b), height_(h), side_(s) {}
    double area() const override { return 0.5 * base_ * height_; }
    double perimeter() const override { return base_ + 2 * side_; }
};

int main() {
    Circle circle(2.0);
    Square square(3.0);
    Triangle triangle(4.0, 5.0, 4.0); // the newly-added type, no existing code changed
    const Shape* shapes[] = { &circle, &square, &triangle };

    for (const Shape* s : shapes)
        std::cout << "area=" << s->area() << " perimeter=" << s->perimeter() << "\n";
    return 0;
}
