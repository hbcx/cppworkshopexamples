#include <iostream>
#include <string>

// Implementor hierarchy.
struct Renderer {
    virtual std::string circle(int r) const = 0;
    virtual std::string square(int side) const = 0;
    virtual ~Renderer() = default;
};

class VectorRenderer : public Renderer {
public:
    std::string circle(int r) const override { return "vector circle(" + std::to_string(r) + ")"; }
    std::string square(int s) const override { return "vector square(" + std::to_string(s) + ")"; }
};

class RasterRenderer : public Renderer {
public:
    std::string circle(int r) const override { return "raster circle(" + std::to_string(r) + ")"; }
    std::string square(int s) const override { return "raster square(" + std::to_string(s) + ")"; }
};

// Abstraction hierarchy: shapes, each holding a renderer and drawing itself
// through it. The two hierarchies meet only at the Renderer interface.
class Shape {
public:
    explicit Shape(const Renderer& r) : renderer_(r) {}
    virtual void draw() const = 0;
    virtual ~Shape() = default;
protected:
    const Renderer& renderer_;
};

class Circle : public Shape {
public:
    Circle(const Renderer& r, int radius) : Shape(r), radius_(radius) {}
    void draw() const override { std::cout << renderer_.circle(radius_) << "\n"; }
private:
    int radius_;
};

class Square : public Shape {
public:
    Square(const Renderer& r, int side) : Shape(r), side_(side) {}
    void draw() const override { std::cout << renderer_.square(side_) << "\n"; }
private:
    int side_;
};

int main() {
    VectorRenderer vec;
    RasterRenderer ras;

    // 2 shapes x 2 renderers = 4 combinations, from only 2 + 2 classes -- not the
    // 4 (CircleVector, CircleRaster, SquareVector, SquareRaster) that combining
    // the two dimensions by inheritance would need. Adding a third renderer is
    // ONE new class that instantly serves every existing shape.
    Circle(vec, 5).draw();
    Circle(ras, 5).draw();
    Square(vec, 3).draw();
    Square(ras, 3).draw();
    return 0;
}
