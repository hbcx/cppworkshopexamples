#include <concepts>
#include <iostream>
#include <string>

template <class T>
concept HasArea = requires(const T& s) {
    { s.area() } -> std::convertible_to<double>;
};
template <class T>
concept HasName = HasArea<T> && requires(const T& s) {
    { s.name() } -> std::convertible_to<std::string>;
};

struct Circle   { double area() const { return 3.14; } };
struct NamedBox { double area() const { return 4.0; } std::string name() const { return "box"; } };

// Three overloads, ordered by how constrained they are.
template <class T>   void describe(const T&)   { std::cout << "some value\n"; }
template <HasArea T> void describe(const T& s) { std::cout << "shape, area=" << s.area() << "\n"; }
template <HasName T> void describe(const T& s) { std::cout << "named " << s.name() << ", area=" << s.area() << "\n"; }

int main() {
    describe(42);            // only the unconstrained overload fits
    describe(Circle{});      // HasArea (Circle has area(), no name())
    describe(NamedBox{});    // HasName subsumes HasArea -> the most constrained wins
    std::cout << "done\n";
}
