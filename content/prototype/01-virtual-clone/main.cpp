#include <iostream>
#include <memory>
#include <string>

// The prototype interface: clone() returns a copy of the concrete type, handed
// back through the base so callers never name Circle or Rectangle.
struct Shape {
    virtual std::unique_ptr<Shape> clone() const = 0;
    virtual void grow() = 0;
    virtual std::string describe() const = 0;
    virtual ~Shape() = default;
};

struct Circle : Shape {
    int r;
    explicit Circle(int radius) : r(radius) {}
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Circle>(*this);   // Circle's copy constructor -> full copy
    }
    void grow() override { r += 1; }
    std::string describe() const override { return "Circle(r=" + std::to_string(r) + ")"; }
};

struct Rectangle : Shape {
    int w, h;
    Rectangle(int width, int height) : w(width), h(height) {}
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Rectangle>(*this);
    }
    void grow() override { w += 1; h += 1; }
    std::string describe() const override {
        return "Rectangle(w=" + std::to_string(w) + ",h=" + std::to_string(h) + ")";
    }
};

// Copies any shape without a switch on its type and without slicing.
std::unique_ptr<Shape> duplicate(const Shape& s) { return s.clone(); }

int main() {
    std::unique_ptr<Shape> circle = std::make_unique<Circle>(5);
    std::unique_ptr<Shape> rect = std::make_unique<Rectangle>(3, 4);

    // Clone through the base interface; grow the clones to prove independence.
    std::unique_ptr<Shape> circleCopy = duplicate(*circle);
    std::unique_ptr<Shape> rectCopy = duplicate(*rect);
    circleCopy->grow();
    rectCopy->grow();

    std::cout << "original: " << circle->describe() << "   clone (grown): " << circleCopy->describe() << '\n';
    std::cout << "original: " << rect->describe() << "   clone (grown): " << rectCopy->describe() << '\n';
    return 0;
}
