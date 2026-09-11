#include <iostream>
#include <typeinfo>

// ANTI-PATTERN: a type-switch that re-implements polymorphism by hand with typeid.
// It is slower than a virtual call, must be edited for every new shape, and
// silently returns a wrong answer for a type it does not list.
struct Shape { virtual ~Shape() = default; };
struct Circle : Shape {};
struct Square : Shape {};

double areaByRtti(const Shape& s) {
    if (typeid(s) == typeid(Circle)) return 3.14;   // stand-in for a real formula
    if (typeid(s) == typeid(Square)) return 4.0;
    return 0.0;                                      // a new shape falls through here
}

// FIX: a virtual function. Each type carries its own answer; adding a shape needs
// no change here, and the compiler makes every shape provide one.
struct Shape2 {
    virtual double area() const = 0;
    virtual ~Shape2() = default;
};
struct Circle2 : Shape2 { double area() const override { return 3.14; } };
struct Square2 : Shape2 { double area() const override { return 4.0; } };

int main() {
    Circle c;
    Square s;
    std::cout << "by RTTI type-switch: " << areaByRtti(c) << " " << areaByRtti(s) << "\n";

    Circle2 c2;
    Square2 s2;
    const Shape2* shapes[] = {&c2, &s2};
    std::cout << "by virtual dispatch: ";
    for (const Shape2* p : shapes)
        std::cout << p->area() << " ";
    std::cout << "\n";
    return 0;
}
