#include <iostream>

// One base with a virtual area(). Each shape overrides it. A Shape& then computes
// the right area for whatever object it refers to, decided at run time.

class Shape {
public:
    virtual double area() const { return 0.0; }   // virtual: overridable
    virtual ~Shape() = default;                   // (needed for safe polymorphic use)
};

class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
private:
    double r_;
};

class Rectangle : public Shape {
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
private:
    double w_, h_;
};

// Works for any Shape, present or future -- it never names Circle or Rectangle.
void printArea(const Shape& s) {
    std::cout << "area = " << s.area() << "\n";
}

int main() {
    Circle c{2.0};
    Rectangle r{3.0, 4.0};

    printArea(c);   // area = 12.5664   (Circle::area chosen at run time)
    printArea(r);   // area = 12        (Rectangle::area)
    return 0;
}
