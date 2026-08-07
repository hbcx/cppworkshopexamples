#include <iostream>
#include <memory>
#include <string>
#include <utility>

// The types we want to draw provide draw() as a FREE function, so they
// need no common base and no member of a fixed name. These are declared
// before the wrapper, so Model<T>::draw() finds them by ordinary lookup.
struct Circle { double r; };
void draw_impl(const Circle& c) { std::cout << "Circle r=" << c.r << "\n"; }

// Pretend this comes from a library we cannot change: no draw member,
// no base class. We adapt it from the OUTSIDE with one free function.
struct LegacyLabel { std::string text; };
void draw_impl(const LegacyLabel& l) { std::cout << "Label " << l.text << "\n"; }

// The erased wrapper: its Model calls the free draw_impl found by
// overload resolution on the stored type.
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
        void draw() const override { draw_impl(data_); }  // free function
        T data_;
    };
    std::unique_ptr<Concept> self_;
};

int main() {
    Drawable a = Circle{2.5};
    Drawable b = LegacyLabel{"hello"};
    a.draw();
    b.draw();

    // LegacyLabel was never touched: no member added, no base class. The
    // only thing written for it was one free draw_impl overload.
    return 0;
}
