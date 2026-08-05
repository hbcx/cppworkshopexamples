// Liskov Substitution Principle -- a subtype must be usable wherever its base
// is expected, with the program still correct.
//
// describe() is written against Shape and names no concrete type. Circle and
// Rectangle each honour area(), so both drop straight in and behave correctly.
// That drop-in property is what LSP asks for -- and it is stricter than virtual
// dispatch: the subtype must do the right THING, not just match the signatures.

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Shape {
public:
    virtual ~Shape() {}
    virtual double area() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
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

// Depends only on the base contract. Any faithful Shape works here.
void describe(const Shape& s) {
    std::cout << s.name() << " area=" << s.area()
              << (s.area() > 10.0 ? " (large)" : " (small)") << "\n";
}

int main() {
    Circle c(2.0);
    Rectangle r(3.0, 4.0);

    // Each subtype substitutes for Shape with no special-casing.
    describe(c);
    describe(r);

    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::unique_ptr<Shape>(new Circle(1.0)));
    shapes.push_back(std::unique_ptr<Shape>(new Rectangle(5.0, 5.0)));
    for (const std::unique_ptr<Shape>& s : shapes) describe(*s);
}
