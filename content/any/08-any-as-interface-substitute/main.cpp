#include <any>
#include <iostream>
#include <memory>
#include <typeinfo>

struct Circle { double r; };
struct Square { double s; };

// Anti-pattern: any holds "some shape", and a type() ladder computes the area.
// The behaviour that belongs ON each shape is pulled out into a switch that
// every such function must repeat and keep in sync as shapes are added -- with
// no compiler check that the ladder is complete.
double areaAny(const std::any& shape) {
    if (shape.type() == typeid(Circle)) { auto c = std::any_cast<Circle>(shape); return 3.14159 * c.r * c.r; }
    if (shape.type() == typeid(Square)) { auto q = std::any_cast<Square>(shape); return q.s * q.s; }
    return 0.0;   // a new shape silently lands here
}

// Fix: give the shapes a common interface and let each compute its own area.
// Adding a shape is one class; a forgotten area() is a compile error.
struct Shape {
    virtual double area() const = 0;
    virtual ~Shape() = default;
};
struct CircleShape : Shape {
    double r;
    explicit CircleShape(double v) : r(v) {}
    double area() const override { return 3.14159 * r * r; }
};

int main() {
    std::cout.setf(std::ios::fixed);
    std::cout.precision(2);

    std::any c = Circle{2.0};
    std::cout << "any ladder: area = " << areaAny(c) << '\n';

    std::unique_ptr<Shape> s = std::make_unique<CircleShape>(2.0);
    std::cout << "interface:  area = " << s->area() << '\n';
    return 0;
}
