#include <iostream>
#include <memory>
#include <string>

// A factory hands back ownership of a polymorphic object. Returning
// unique_ptr<Shape> makes ownership explicit and transferable: the caller owns
// exactly one Shape and cannot forget to delete it.
class Shape {
public:
    virtual ~Shape() { std::cout << "~Shape\n"; }   // virtual: deleting through a
    virtual double area() const = 0;                // Shape* must destroy the
    virtual const char* name() const = 0;           // concrete type
};

class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}
    ~Circle() override { std::cout << "~Circle\n"; }
    double area() const override { return 3.14159265 * r_ * r_; }
    const char* name() const override { return "circle"; }
private:
    double r_;
};

class Square : public Shape {
public:
    explicit Square(double s) : s_(s) {}
    ~Square() override { std::cout << "~Square\n"; }
    double area() const override { return s_ * s_; }
    const char* name() const override { return "square"; }
private:
    double s_;
};

static std::unique_ptr<Shape> makeShape(const std::string& kind) {
    if (kind == "circle") {
        return std::unique_ptr<Shape>(new Circle(2.0));
    }
    return std::unique_ptr<Shape>(new Square(3.0));
}

int main() {
    std::unique_ptr<Shape> s = makeShape("circle");
    std::cout << s->name() << " area = " << s->area() << '\n';

    s = makeShape("square");     // the previous Circle is deleted here
    std::cout << s->name() << " area = " << s->area() << '\n';
    return 0;                    // the Square is deleted here
}
