#include <iostream>
#include <string>

// Implementor: the low-level interface the abstraction delegates to. Rendering
// backends implement this, and they know nothing about shapes.
struct Renderer {
    virtual std::string renderCircle(int radius) const = 0;
    virtual ~Renderer() = default;
};

// Concrete implementors: two independent backends.
class VectorRenderer : public Renderer {
public:
    std::string renderCircle(int radius) const override {
        return "vector circle of radius " + std::to_string(radius);
    }
};

class RasterRenderer : public Renderer {
public:
    std::string renderCircle(int radius) const override {
        return "raster circle of radius " + std::to_string(radius);
    }
};

// Abstraction: holds a reference to an Implementor and expresses its operation in
// terms of the implementor's primitives. Circle does not know or care which
// renderer it has -- the reference IS the bridge between the two hierarchies, and
// it lets the shape and the renderer vary independently.
class Circle {
public:
    Circle(const Renderer& r, int radius) : renderer_(r), radius_(radius) {}
    void draw() const { std::cout << renderer_.renderCircle(radius_) << "\n"; }
private:
    const Renderer& renderer_;
    int radius_;
};

int main() {
    VectorRenderer vector;
    RasterRenderer raster;

    // The same abstraction (Circle) runs over either implementation, chosen at
    // runtime -- neither side knows the other's concrete type.
    Circle(vector, 5).draw();     // vector circle of radius 5
    Circle(raster, 5).draw();     // raster circle of radius 5
    return 0;
}
