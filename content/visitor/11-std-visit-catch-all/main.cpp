// Anti-pattern: a generic catch-all in std::visit.
//
// The best property of std::visit is compile-time exhaustiveness: if the overload
// set does not cover every alternative, the code does not compile. A generic
// [](const auto&) catch-all throws that away -- it matches every type, so the
// overload set is always "complete", and a new alternative you meant to handle
// specifically is silently absorbed by the catch-all with default behaviour. The
// fix is an explicit lambda per alternative, so a missing case is a compile error
// again.

#include <iostream>
#include <variant>
#include <vector>

struct Circle { double radius; };
struct Square { double side; };
struct Triangle { double base; double height; };

using Shape = std::variant<Circle, Square, Triangle>;

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

int main() {
    std::vector<Shape> shapes = { Circle{ 2.0 }, Square{ 3.0 }, Triangle{ 4.0, 5.0 } };

    std::cout << "bad (auto catch-all swallows Square and Triangle):\n";
    for (const Shape& shape : shapes) {
        double area = std::visit(overloaded{
            [](const Circle& c) { return 3.14159 * c.radius * c.radius; },
            [](const auto&)     { return -1.0; }, // silently handles everything else
        }, shape);
        std::cout << "  area = " << area << "\n";
    }

    std::cout << "good (explicit case per alternative):\n";
    for (const Shape& shape : shapes) {
        double area = std::visit(overloaded{
            [](const Circle& c)   { return 3.14159 * c.radius * c.radius; },
            [](const Square& s)   { return s.side * s.side; },
            [](const Triangle& t) { return 0.5 * t.base * t.height; },
        }, shape);
        std::cout << "  area = " << area << "\n";
    }
    return 0;
}
