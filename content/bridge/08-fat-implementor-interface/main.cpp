#include <iostream>

// ANTI-PATTERN: the implementor has a method per ABSTRACTION (per shape), so the
// two hierarchies are coupled at the interface. Adding a Triangle shape forces a
// new drawTriangle() onto the Renderer interface AND onto every concrete renderer
// below it -- an abstraction-side change that churns the whole implementation
// hierarchy, defeating the independence a Bridge is meant to give.
struct FatRenderer {
    virtual void drawCircle() = 0;
    virtual void drawSquare() = 0;
    // a new shape means a new method here, and in every renderer
    virtual ~FatRenderer() = default;
};

class FatVector : public FatRenderer {
public:
    void drawCircle() override { std::cout << "vector: circle\n"; }
    void drawSquare() override { std::cout << "vector: square\n"; }
};

// FIX: the implementor offers PRIMITIVES that know nothing of shapes; each shape
// composes them. A new shape composes existing primitives and adds NOTHING to the
// renderer, so the implementation hierarchy is untouched by abstraction-side
// growth -- the two axes are free again.
struct LeanRenderer {
    virtual void line() = 0;
    virtual void arc() = 0;
    virtual ~LeanRenderer() = default;
};

class LeanVector : public LeanRenderer {
public:
    void line() override { std::cout << "vector: line\n"; }
    void arc() override { std::cout << "vector: arc\n"; }
};

class Circle {
public:
    explicit Circle(LeanRenderer& r) : r_(r) {}
    void draw() { r_.arc(); }                        // composed from primitives
private:
    LeanRenderer& r_;
};

class Triangle {                                     // a NEW shape -- LeanRenderer unchanged
public:
    explicit Triangle(LeanRenderer& r) : r_(r) {}
    void draw() { r_.line(); r_.line(); r_.line(); }
private:
    LeanRenderer& r_;
};

int main() {
    std::cout << "== fat implementor: a method per shape ==\n";
    FatVector fv;
    fv.drawCircle();
    fv.drawSquare();

    std::cout << "== lean implementor: primitives the shapes compose ==\n";
    LeanVector lv;
    Circle(lv).draw();
    Triangle(lv).draw();      // a new shape, and no new renderer method
    return 0;
}
