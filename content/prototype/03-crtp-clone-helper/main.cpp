#include <iostream>
#include <memory>
#include <string>

// Base prototype interface.
struct Shape {
    virtual std::unique_ptr<Shape> clone() const = 0;
    virtual std::string describe() const = 0;
    virtual ~Shape() = default;
};

// CRTP helper: implements clone() ONCE using its template argument to name the
// concrete type. Any leaf class deriving from CloneableShape<Itself> gets a
// correct clone with no hand-written body.
template <class Derived>
struct CloneableShape : Shape {
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Derived>(static_cast<const Derived&>(*this));
    }
};

// Concrete shapes: note there is NO clone() here -- it comes from the helper.
struct Circle : CloneableShape<Circle> {
    int r;
    explicit Circle(int radius) : r(radius) {}
    std::string describe() const override { return "Circle(r=" + std::to_string(r) + ")"; }
};

struct Rectangle : CloneableShape<Rectangle> {
    int w, h;
    Rectangle(int width, int height) : w(width), h(height) {}
    std::string describe() const override {
        return "Rectangle(w=" + std::to_string(w) + ",h=" + std::to_string(h) + ")";
    }
};

int main() {
    std::unique_ptr<Shape> shapes[] = {
        std::make_unique<Circle>(7),
        std::make_unique<Rectangle>(2, 9),
    };

    // Clone through the base: the helper produced a clone() that returns the
    // exact concrete type for each, no boilerplate in Circle or Rectangle.
    for (const std::unique_ptr<Shape>& s : shapes) {
        std::unique_ptr<Shape> copy = s->clone();
        std::cout << "cloned " << copy->describe() << '\n';
    }
    return 0;
}
