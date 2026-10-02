#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <new>

static int g_allocs = 0;
void* operator new(std::size_t n) {
    ++g_allocs;
    if (void* p = std::malloc(n ? n : 1)) return p;
    throw std::bad_alloc();
}
void operator delete(void* p) noexcept              { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

// A closed set as a tagged union: the value lives inline, no allocation, no vtable.
struct Shape {
    enum class Kind { Circle, Square } tag;
    union {
        double radius;
        double side;
    };
    double area() const {
        switch (tag) {
            case Kind::Circle: return 3.14159 * radius * radius;
            case Kind::Square: return side * side;
        }
        return 0.0;
    }
};

// The same closed set as a class hierarchy: each object is heap-allocated behind
// a base pointer and carries a vtable pointer.
struct ShapeBase {
    virtual double area() const = 0;
    virtual ~ShapeBase() = default;
};
struct Circle : ShapeBase { double r; explicit Circle(double x) : r(x) {} double area() const override { return 3.14159 * r * r; } };
struct Square : ShapeBase { double s; explicit Square(double x) : s(x) {} double area() const override { return s * s; } };

int main() {
    // Tagged union: constructed inline, no allocation.
    g_allocs = 0;
    Shape c;
    c.tag = Shape::Kind::Circle;
    c.radius = 2.0;
    double taggedArea = c.area();
    int taggedAllocs = g_allocs;

    // Polymorphic: one heap allocation per object, plus a vtable pointer.
    g_allocs = 0;
    std::unique_ptr<ShapeBase> p = std::make_unique<Circle>(2.0);
    double polyArea = p->area();
    int polyAllocs = g_allocs;

    std::cout << "tagged union area = " << taggedArea
              << ", allocations: " << taggedAllocs << '\n';       // 12.5664, 0
    std::cout << "polymorphic area = " << polyArea
              << ", allocations: " << polyAllocs << '\n';         // 12.5664, 1
    std::cout << "sizeof(tagged Shape) = " << sizeof(Shape)
              << " (inline), sizeof(base pointer) = " << sizeof(ShapeBase*)
              << " + a heap object with a vtable\n";
    std::cout << "closed set: inline, no vtable; open set: heap, virtual dispatch\n";
    return 0;
}
