// When the relationship is not a behavioural is-a, do not inherit -- compose,
// or share an interface.
//
// A square IS a shape, but it is NOT a rectangle you can resize on two axes.
// So Rectangle and Square are siblings under Shape, each with its own area
// rule. There is no inherited setWidth/setHeight for a square to violate, so
// both substitute for Shape safely. Where a type must reuse another's guts,
// Square below COMPOSES a rectangle and exposes only a coherent setSide.

#include <iostream>
#include <string>

class Shape {
public:
    virtual ~Shape() {}
    virtual int area() const = 0;
    virtual std::string name() const = 0;
};

class Rectangle : public Shape {
public:
    Rectangle(int w, int h) : w_(w), h_(h) {}
    void setWidth(int w) { w_ = w; }   // two independent axes -- a real promise
    void setHeight(int h) { h_ = h; }
    int area() const override { return w_ * h_; }
    std::string name() const override { return "rectangle"; }
private:
    int w_;
    int h_;
};

// A sibling of Rectangle, not a subtype. It COMPOSES a rectangle but exposes
// only setSide, so the always-equal-sides invariant cannot be broken.
class Square : public Shape {
public:
    explicit Square(int side) : rect_(side, side) {}
    void setSide(int side) { rect_ = Rectangle(side, side); }
    int area() const override { return rect_.area(); }
    std::string name() const override { return "square"; }
private:
    Rectangle rect_;
};

void report(const Shape& s) {
    std::cout << s.name() << " area=" << s.area() << "\n";
}

int main() {
    Rectangle r(3, 4);
    r.setWidth(5);
    r.setHeight(4); // independent axes: area = 20, exactly as a caller expects
    report(r);

    Square sq(3);
    sq.setSide(5); // one coherent operation: area = 25
    report(sq);

    // Both are Shapes and substitute wherever a Shape is expected; neither can
    // surprise a caller, because neither inherits the other's resize promise.
}
