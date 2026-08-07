#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Three plain shapes. name() and area() are free functions, so the types
// stay simple and unrelated.
struct Circle   { double r; };
struct Square   { double side; };
struct Triangle { double base; double height; };

std::string shapeName(const Circle&)   { return "Circle"; }
std::string shapeName(const Square&)   { return "Square"; }
std::string shapeName(const Triangle&) { return "Triangle"; }

double shapeArea(const Circle& c)   { return 3.14159 * c.r * c.r; }
double shapeArea(const Square& s)   { return s.side * s.side; }
double shapeArea(const Triangle& t) { return 0.5 * t.base * t.height; }

class Shape {
public:
    template <class T>
    Shape(T x) : self_(new Model<T>(std::move(x))) {}

    std::string name() const { return self_->name(); }
    double area() const { return self_->area(); }   // GOOD: part of the interface

    // Escape hatch: recover the concrete type. Its presence is a smell.
    template <class T>
    const T* target() const {
        if (const Model<T>* m = dynamic_cast<const Model<T>*>(self_.get()))
            return &m->data_;
        return nullptr;
    }

private:
    struct Concept {
        virtual ~Concept() = default;
        virtual std::string name() const = 0;
        virtual double area() const = 0;
    };
    template <class T>
    struct Model : Concept {
        Model(T x) : data_(std::move(x)) {}
        std::string name() const override { return shapeName(data_); }
        double area() const override { return shapeArea(data_); }
        T data_;
    };
    std::unique_ptr<Concept> self_;
};

// BAD: switch on the concrete type behind the erased value -- exactly what
// type erasure was meant to remove. Every new shape needs another branch.
double areaDowncast(const Shape& s) {
    if (const Circle* c = s.target<Circle>()) return 3.14159 * c->r * c->r;
    if (const Square* q = s.target<Square>()) return q->side * q->side;
    // Triangle was added later and nobody updated this switch.
    return 0.0;
}

int main() {
    std::vector<Shape> shapes;
    shapes.push_back(Circle{1.0});
    shapes.push_back(Square{2.0});
    shapes.push_back(Triangle{3.0, 4.0});

    std::cout << "good -- area() in the interface:\n";
    for (const Shape& s : shapes)
        std::cout << "  " << s.name() << " area=" << s.area() << "\n";

    std::cout << "bad -- downcast switch:\n";
    for (const Shape& s : shapes)
        std::cout << "  " << s.name() << " area=" << areaDowncast(s) << "\n";

    // Triangle prints area=0 in the bad path: the type switch forgot it,
    // and a forgotten branch is a silent wrong answer, not a compile error.
    return 0;
}
