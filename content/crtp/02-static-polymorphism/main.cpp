// Static polymorphism (CRTP) versus dynamic polymorphism (virtual), side by
// side, so the trade-off is concrete.
//
// Both share one describe() that defers area() to a per-type step. The virtual
// version dispatches at run time and can be stored behind one base pointer. The
// CRTP version dispatches at compile time -- inlined, no vtable -- but its
// instantiations are unrelated types you cannot store together.

#include <iostream>
#include <memory>
#include <vector>

const double kPi = 3.14159;

// ---------- dynamic: one base, runtime dispatch, uniform storage ----------
namespace dynamic {

class VShape {
public:
    virtual ~VShape() {}
    virtual double area() const = 0;
    void describe() const { std::cout << "  area = " << area() << "\n"; }
};

class Circle : public VShape {
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return kPi * r_ * r_; }
private:
    double r_;
};

class Square : public VShape {
public:
    explicit Square(double s) : s_(s) {}
    double area() const override { return s_ * s_; }
private:
    double s_;
};

} // namespace dynamic

// ---------- static: CRTP base, compile-time dispatch, no vtable ----------
namespace static_poly {

template <class Derived>
class Shape {
public:
    void describe() const {
        std::cout << "  area = " << static_cast<const Derived&>(*this).area() << "\n";
    }
};

struct Circle : Shape<Circle> {
    explicit Circle(double r) : r(r) {}
    double area() const { return kPi * r * r; }
    double r;
};

struct Square : Shape<Square> {
    explicit Square(double s) : s(s) {}
    double area() const { return s * s; }
    double s;
};

} // namespace static_poly

int main() {
    // Dynamic: circles and squares share one container.
    std::cout << "dynamic (virtual):\n";
    std::vector<std::unique_ptr<dynamic::VShape>> shapes;
    shapes.push_back(std::unique_ptr<dynamic::VShape>(new dynamic::Circle(2.0)));
    shapes.push_back(std::unique_ptr<dynamic::VShape>(new dynamic::Square(3.0)));
    for (const std::unique_ptr<dynamic::VShape>& s : shapes) s->describe();

    // Static: each call inlines, but the types have no common base.
    std::cout << "static (CRTP):\n";
    static_poly::Circle c(2.0);
    static_poly::Square q(3.0);
    c.describe();
    q.describe();
}
