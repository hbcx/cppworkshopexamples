#include <iostream>
#include <memory>
#include <string>

// A small polymorphic hierarchy. A shared_ptr can be cast up and down this
// hierarchy WITHOUT losing shared ownership: each *_pointer_cast helper returns
// a new shared_ptr that shares the SAME control block, so there is one shared
// reference count across the original and every cast result.
struct Shape {
    virtual ~Shape() = default;
    virtual std::string kind() const { return "Shape"; }
};

struct Circle : Shape {
    double radius;
    explicit Circle(double r) : radius(r) {}
    std::string kind() const override { return "Circle"; }
};

struct Square : Shape {
    double side;
    explicit Square(double s) : side(s) {}
    std::string kind() const override { return "Square"; }
};

int main() {
    std::shared_ptr<Shape> shape = std::make_shared<Circle>(2.0);

    // static_pointer_cast: a downcast you make when you already KNOW the dynamic
    // type. There is no runtime check -- it is the shared_ptr analog of
    // static_cast. The result shares the control block, so use_count is now 2.
    std::shared_ptr<Circle> circle = std::static_pointer_cast<Circle>(shape);
    std::cout << "static_pointer_cast -> " << circle->kind()
              << ", radius " << circle->radius << '\n';
    std::cout << "shared count after cast = " << shape.use_count() << '\n';   // 2

    // dynamic_pointer_cast: a CHECKED downcast (the type must be polymorphic).
    // On a type mismatch it returns an EMPTY shared_ptr instead of a bad
    // pointer -- the analog of dynamic_cast returning nullptr.
    std::shared_ptr<Square> asSquare = std::dynamic_pointer_cast<Square>(shape);
    std::cout << "dynamic_pointer_cast to the wrong type is "
              << (asSquare ? "non-empty" : "empty") << '\n';

    std::shared_ptr<Circle> asCircle = std::dynamic_pointer_cast<Circle>(shape);
    std::cout << "dynamic_pointer_cast to the right type is "
              << (asCircle ? "non-empty" : "empty") << '\n';

    // const_pointer_cast: add or strip const on the pointed-to type, again
    // sharing the same object and the same control block.
    std::shared_ptr<const Circle> readOnly = circle;
    std::shared_ptr<Circle> writable = std::const_pointer_cast<Circle>(readOnly);
    writable->radius = 3.5;
    std::cout << "const_pointer_cast let us write radius = " << circle->radius << '\n';

    // shape, circle, asCircle, readOnly and writable all share one object.
    std::cout << "final shared count = " << shape.use_count() << '\n';        // 5
    return 0;
}
