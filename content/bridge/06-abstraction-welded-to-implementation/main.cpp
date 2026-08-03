#include <iostream>
#include <string>

struct Renderer {
    virtual std::string circle(int r) const = 0;
    virtual ~Renderer() = default;
};

class VectorRenderer : public Renderer {
public:
    std::string circle(int r) const override { return "vector circle(" + std::to_string(r) + ")"; }
};

class RasterRenderer : public Renderer {
public:
    std::string circle(int r) const override { return "raster circle(" + std::to_string(r) + ")"; }
};

// ANTI-PATTERN: the abstraction is written against the CONCRETE implementor, not
// the Renderer interface. The bridge is nominal -- this Circle is welded to
// VectorRenderer and can never be given a RasterRenderer, so the two hierarchies
// cannot vary independently, which was the entire reason to use a Bridge.
class CircleBad {
public:
    CircleBad(const VectorRenderer& r, int radius) : r_(r), radius_(radius) {}   // concrete -- BUG
    void draw() const { std::cout << r_.circle(radius_) << "\n"; }
private:
    const VectorRenderer& r_;      // cannot hold any other renderer
    int radius_;
};

// FIX: depend on the implementor INTERFACE. The same Circle now runs over any
// renderer, and the two hierarchies are genuinely independent.
class CircleGood {
public:
    CircleGood(const Renderer& r, int radius) : r_(r), radius_(radius) {}
    void draw() const { std::cout << r_.circle(radius_) << "\n"; }
private:
    const Renderer& r_;
    int radius_;
};

int main() {
    VectorRenderer vec;
    RasterRenderer ras;

    std::cout << "== welded to the concrete implementor ==\n";
    CircleBad(vec, 5).draw();
    // CircleBad(ras, 5) would not compile -- it only accepts a VectorRenderer.

    std::cout << "== depends on the interface: any backend works ==\n";
    CircleGood(vec, 5).draw();
    CircleGood(ras, 5).draw();
    return 0;
}
