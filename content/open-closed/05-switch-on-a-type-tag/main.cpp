// Anti-pattern: a switch on a type tag.
//
// Each shape carries a Kind, and every operation switches on it. Adding a shape
// means editing area(), name(), and every other such switch -- and missing one
// is a silent wrong answer, not a compile error, unless a warning happens to
// catch it.
//
// The fix is polymorphism: the type IS the dispatch, and a new shape is a new
// class that touches none of the existing code.

#include <iostream>
#include <memory>
#include <string>
#include <vector>

const double kPi = 3.14159;

// ---------- a tag, and a switch per operation ----------
namespace bad {

enum class Kind { Circle, Rectangle };

struct Shape {
    Kind kind;
    double a; // radius, or width
    double b; // height (rectangle)
};

double area(const Shape& s) {
    switch (s.kind) {
        case Kind::Circle:    return kPi * s.a * s.a;
        case Kind::Rectangle: return s.a * s.b;
    }
    return 0.0; // adding Kind::Triangle forces a new case in EVERY such switch
}

std::string name(const Shape& s) {
    switch (s.kind) {
        case Kind::Circle:    return "circle";
        case Kind::Rectangle: return "rectangle";
    }
    return "?";
}

} // namespace bad

// ---------- the type is the dispatch ----------
namespace good {

class Shape {
public:
    virtual ~Shape() {}
    virtual double area() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return kPi * r_ * r_; }
    std::string name() const override { return "circle"; }
private:
    double r_;
};

class Rectangle : public Shape {
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    std::string name() const override { return "rectangle"; }
private:
    double w_;
    double h_;
};

// A new shape: no existing class or function changes.
class Triangle : public Shape {
public:
    Triangle(double base, double height) : base_(base), height_(height) {}
    double area() const override { return 0.5 * base_ * height_; }
    std::string name() const override { return "triangle"; }
private:
    double base_;
    double height_;
};

} // namespace good

int main() {
    bad::Shape bs{bad::Kind::Circle, 1.0, 0.0};
    std::cout << "[bad]  " << bad::name(bs) << " area=" << bad::area(bs) << "\n";

    std::vector<std::unique_ptr<good::Shape>> shapes;
    shapes.push_back(std::unique_ptr<good::Shape>(new good::Circle(1.0)));
    shapes.push_back(std::unique_ptr<good::Shape>(new good::Rectangle(3.0, 4.0)));
    shapes.push_back(std::unique_ptr<good::Shape>(new good::Triangle(6.0, 4.0)));
    for (const std::unique_ptr<good::Shape>& s : shapes)
        std::cout << "[good] " << s->name() << " area=" << s->area() << "\n";
}
