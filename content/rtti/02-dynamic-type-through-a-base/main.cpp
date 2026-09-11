#include <iostream>
#include <typeinfo>

struct Shape { virtual ~Shape() = default; };   // polymorphic: it has a virtual
struct Circle : Shape {};
struct Square : Shape {};

int main() {
    Circle c;
    Square s;
    Shape* shapes[] = {&c, &s};

    // For a POLYMORPHIC type, typeid on the dereferenced base pointer reports the
    // MOST-DERIVED (dynamic) type, not the static Shape.
    for (Shape* p : shapes)
        std::cout << "runtime type: " << typeid(*p).name() << "\n";

    std::cout << std::boolalpha;
    std::cout << "shapes[0] is a Circle? " << (typeid(*shapes[0]) == typeid(Circle)) << "\n";
    std::cout << "shapes[1] is a Circle? " << (typeid(*shapes[1]) == typeid(Circle)) << "\n";
    return 0;
}
