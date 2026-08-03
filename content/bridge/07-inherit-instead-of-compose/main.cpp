#include <iostream>
#include <string>

// Two rendering backends.
class VectorBackend {
public:
    std::string circle(int r) const { return "vector circle(" + std::to_string(r) + ")"; }
};

class RasterBackend {
public:
    std::string circle(int r) const { return "raster circle(" + std::to_string(r) + ")"; }
};

// ANTI-PATTERN: the shape INHERITS its implementation instead of holding it. Each
// (shape, backend) pair becomes its own class -- CircleVector and CircleRaster
// here, and a Square would need SquareVector and SquareRaster too. That is the
// M x N class explosion a Bridge exists to avoid, re-created by choosing
// inheritance over composition.
class CircleVector : public VectorBackend {
public:
    explicit CircleVector(int r) : r_(r) {}
    void draw() const { std::cout << circle(r_) << "\n"; }
private:
    int r_;
};

class CircleRaster : public RasterBackend {
public:
    explicit CircleRaster(int r) : r_(r) {}
    void draw() const { std::cout << circle(r_) << "\n"; }
private:
    int r_;
};

// FIX: the shape HOLDS an implementor interface. One Circle class serves every
// backend, and a new backend is one class that serves every shape -- M + N, not
// M x N.
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

class Circle {
public:
    Circle(const Renderer& r, int radius) : r_(r), radius_(radius) {}
    void draw() const { std::cout << r_.circle(radius_) << "\n"; }
private:
    const Renderer& r_;
    int radius_;
};

int main() {
    std::cout << "== inheritance: a class per (shape, backend) pair ==\n";
    CircleVector(5).draw();
    CircleRaster(5).draw();

    std::cout << "== composition: one Circle over any backend ==\n";
    VectorRenderer vec;
    RasterRenderer ras;
    Circle(vec, 5).draw();
    Circle(ras, 5).draw();
    return 0;
}
