// The modern C++ Visitor: std::variant + std::visit.
//
// When the set of element types is closed and known, std::variant holds one of
// them and std::visit dispatches on whichever it currently holds -- no base
// class, no accept, no virtual. The visitor is an overload set of lambdas (built
// with the "overloaded" idiom), one per alternative. This is the idiomatic form
// of Visitor in modern C++, and its exhaustiveness is checked at compile time:
// leave out an alternative and it will not compile (see the catch-all
// anti-pattern for how that safety is lost).

#include <iostream>
#include <variant>
#include <vector>

struct Circle { double radius; };
struct Square { double side; };
struct Triangle { double base; double height; };

using Shape = std::variant<Circle, Square, Triangle>;

// The overloaded idiom: inherit operator() from a pack of lambdas.
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>; // deduction guide (needed in C++17)

int main() {
    std::vector<Shape> shapes = { Circle{ 2.0 }, Square{ 3.0 }, Triangle{ 4.0, 5.0 } };

    for (const Shape& shape : shapes) {
        double area = std::visit(overloaded{
            [](const Circle& c)   { return 3.14159 * c.radius * c.radius; },
            [](const Square& s)   { return s.side * s.side; },
            [](const Triangle& t) { return 0.5 * t.base * t.height; },
        }, shape);
        std::cout << "area = " << area << "\n";
    }
    return 0;
}
