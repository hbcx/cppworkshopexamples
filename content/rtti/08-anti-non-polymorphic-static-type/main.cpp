#include <iostream>
#include <typeinfo>

// A base with NO virtual functions -- not polymorphic, so it carries no run-time
// type data.
struct Animal {
    int legs = 4;
};
struct Snake : Animal {
    Snake() { legs = 0; }
};

int main() {
    Snake snake;
    Animal* a = &snake;

    std::cout << std::boolalpha;

    // ANTI-PATTERN: expecting typeid(*a) to report Snake. Animal has no virtual
    // functions, so there is no RTTI to consult and typeid uses the STATIC type of
    // the expression -- Animal, not the real Snake.
    std::cout << "typeid(*a).name() = " << typeid(*a).name()
              << "   (static type Animal, not Snake)\n";
    std::cout << "is *a a Snake?    " << (typeid(*a) == typeid(Snake))
              << "   (false -- wrong!)\n";

    // FIX: make the base polymorphic (give it a virtual function, e.g. a virtual
    // destructor). Then typeid(*a) reports the dynamic type -- see the
    // dynamic-type-through-a-base example, where Shape has a virtual destructor.
    return 0;
}
