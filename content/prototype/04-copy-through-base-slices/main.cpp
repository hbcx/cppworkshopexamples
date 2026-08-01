#include <iostream>
#include <memory>
#include <string>

// A concrete (instantiable) base, so we can actually make a base VALUE and watch
// it slice. clone() is the virtual copy that does not slice.
struct Shape {
    virtual std::string describe() const { return "Shape(base)"; }
    virtual std::unique_ptr<Shape> clone() const { return std::make_unique<Shape>(*this); }
    virtual ~Shape() = default;
};

struct Circle : Shape {
    int r;
    explicit Circle(int radius) : r(radius) {}
    std::string describe() const override { return "Circle(r=" + std::to_string(r) + ")"; }
    std::unique_ptr<Shape> clone() const override { return std::make_unique<Circle>(*this); }
};

int main() {
    Circle circle(5);

    // Anti-pattern: copy a Circle into a base VALUE. Only the Shape sub-object is
    // copied -- the Circle part is sliced off, and nothing warns.
    Shape sliced = circle;
    std::cout << "base-value copy: " << sliced.describe() << '\n';   // Shape(base)

    // Fix: clone() through the interface keeps the concrete type and its data.
    std::unique_ptr<Shape> cloned = circle.clone();
    std::cout << "clone() copy:    " << cloned->describe() << '\n';  // Circle(r=5)
    return 0;
}
