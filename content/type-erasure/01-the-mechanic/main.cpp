#include <iostream>
#include <memory>
#include <utility>

// A Drawable value that can hold ANY type with a draw() member, without
// that type deriving from a common base. The magic is a private pair:
// an abstract Concept and a Model<T> that forwards to the stored object.
class Drawable {
public:
    template <class T>
    Drawable(T x) : self_(new Model<T>(std::move(x))) {}

    void draw() const { self_->draw(); }

private:
    struct Concept {
        virtual ~Concept() = default;
        virtual void draw() const = 0;
    };
    template <class T>
    struct Model : Concept {
        Model(T x) : data_(std::move(x)) {}
        void draw() const override { data_.draw(); }
        T data_;
    };
    std::unique_ptr<Concept> self_;   // owns the erased object as a Concept*
};

// Two unrelated types. Neither inherits from anything, neither mentions
// Drawable, yet both fit into one Drawable value.
struct Circle {
    double r;
    void draw() const { std::cout << "Circle r=" << r << "\n"; }
};
struct Square {
    double s;
    void draw() const { std::cout << "Square s=" << s << "\n"; }
};

int main() {
    Drawable a = Circle{1.5};
    Drawable b = Square{2.0};
    a.draw();
    b.draw();

    // Same static type, so they can share a container (see example 03 for
    // the copyable version that a std::vector really wants).
    Drawable c = std::move(a);
    c.draw();
    return 0;
}
