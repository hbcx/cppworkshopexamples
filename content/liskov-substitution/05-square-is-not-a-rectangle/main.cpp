// Anti-pattern: a square is not a rectangle (the textbook LSP violation).
//
// Rectangle promises two independent axes. Square inherits it and, to keep its
// sides equal, overrides setWidth/setHeight to change BOTH sides. A function
// written against Rectangle sets 5x4 and expects area 20 -- and gets 16 from a
// Square. It compiles; the break is behavioural.
//
// The fix: make Rectangle and Square siblings under a Shape interface, with no
// shared two-axis setter to violate.

#include <iostream>
#include <string>

// ---------- Square inherits Rectangle and breaks its promise ----------
namespace bad {

class Rectangle {
public:
    Rectangle(int w, int h) : w_(w), h_(h) {}
    virtual ~Rectangle() {}
    virtual void setWidth(int w) { w_ = w; }
    virtual void setHeight(int h) { h_ = h; }
    int area() const { return w_ * h_; }
protected:
    int w_;
    int h_;
};

class Square : public Rectangle {
public:
    explicit Square(int s) : Rectangle(s, s) {}
    void setWidth(int w) override { w_ = w; h_ = w; }  // changes BOTH sides
    void setHeight(int h) override { w_ = h; h_ = h; }
};

// Written against Rectangle's two-axis promise.
void resizeAndCheck(Rectangle& r, const std::string& what) {
    r.setWidth(5);
    r.setHeight(4);
    std::cout << "[bad] " << what << ": expected 20, got " << r.area() << "\n";
}

} // namespace bad

// ---------- siblings under Shape, no promise to break ----------
namespace good {

class Shape {
public:
    virtual ~Shape() {}
    virtual int area() const = 0;
    virtual std::string name() const = 0;
};

class Rectangle : public Shape {
public:
    Rectangle(int w, int h) : w_(w), h_(h) {}
    int area() const override { return w_ * h_; }
    std::string name() const override { return "rectangle"; }
private:
    int w_;
    int h_;
};

class Square : public Shape {
public:
    explicit Square(int s) : s_(s) {}
    int area() const override { return s_ * s_; }
    std::string name() const override { return "square"; }
private:
    int s_;
};

} // namespace good

int main() {
    bad::Rectangle r(1, 1);
    bad::Square sq(1);
    resizeAndCheck(r, "rectangle");  // 20, as promised
    resizeAndCheck(sq, "square");    // 16 -- substitution broken

    good::Rectangle gr(5, 4);
    good::Square gsq(5);
    std::cout << "[good] " << gr.name() << " area=" << gr.area() << "\n";
    std::cout << "[good] " << gsq.name() << " area=" << gsq.area() << "\n";
}
