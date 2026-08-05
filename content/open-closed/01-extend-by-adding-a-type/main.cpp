// Open-Closed Principle -- open for extension, closed for modification.
//
// New behaviour should arrive as NEW code, not edits to code that already
// works. Here an area report sums the area of any Shape through a virtual
// method. Adding a Triangle is a new class; the report -- the closed part --
// never changes and cannot regress the shapes it already handled.

#include <iostream>
#include <memory>
#include <string>
#include <vector>

const double kPi = 3.14159;

// The extension seam: every shape is used only through this interface.
class Shape {
public:
    virtual ~Shape() {}
    virtual double area() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return kPi * r_ * r_; }
    std::string name() const override { return "circle"; }
private:
    double r_;
};

class Rectangle : public Shape {
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    std::string name() const override { return "rectangle"; }
private:
    double w_;
    double h_;
};

// The CLOSED part: works for any Shape, present or future. It is never edited
// when a new shape appears.
class AreaReport {
public:
    void add(std::unique_ptr<Shape> s) { shapes_.push_back(std::move(s)); }

    double total() const {
        double t = 0.0;
        for (const std::unique_ptr<Shape>& s : shapes_) t += s->area();
        return t;
    }

    void print() const {
        for (const std::unique_ptr<Shape>& s : shapes_)
            std::cout << "  " << s->name() << ": " << s->area() << "\n";
        std::cout << "  total: " << total() << "\n";
    }

private:
    std::vector<std::unique_ptr<Shape>> shapes_;
};

// --- EXTENSION: a new shape added later, with NO change above this line. ---
class Triangle : public Shape {
public:
    Triangle(double base, double height) : base_(base), height_(height) {}
    double area() const override { return 0.5 * base_ * height_; }
    std::string name() const override { return "triangle"; }
private:
    double base_;
    double height_;
};

int main() {
    AreaReport report;
    report.add(std::unique_ptr<Shape>(new Circle(1.0)));
    report.add(std::unique_ptr<Shape>(new Rectangle(3.0, 4.0)));
    report.add(std::unique_ptr<Shape>(new Triangle(6.0, 4.0))); // the new type
    report.print();
}
