// Compile-time Template Method with CRTP: no virtual, no overhead.
//
// When the varying step is fixed per type rather than chosen at runtime, the
// skeleton does not need virtual dispatch. The Curiously Recurring Template
// Pattern (CRTP) makes the base a template over its own derived class, so the
// skeleton can call the derived step through a static_cast resolved entirely at
// compile time. The algorithm's shape still lives once in the base, subclasses
// still supply only the steps -- but the calls inline, so the abstraction costs
// nothing. The trade-off vs the virtual form is in the notes.

#include <iostream>

template <class Derived>
class Beverage {
public:
    // The template method (skeleton). It calls the steps on the derived type,
    // reached by casting this to Derived* -- a compile-time, zero-cost dispatch.
    void prepare() {
        boilWater();
        self().brew();      // resolved at compile time to Derived::brew
        pourInCup();
        self().addExtras(); // resolved at compile time to Derived::addExtras
    }

private:
    Derived& self() { return static_cast<Derived&>(*this); }
    void boilWater() { std::cout << "boil water\n"; }
    void pourInCup() { std::cout << "pour into cup\n"; }
};

class Tea : public Beverage<Tea> {
public:
    void brew()      { std::cout << "steep the tea\n"; }
    void addExtras() { std::cout << "add lemon\n"; }
};

class Coffee : public Beverage<Coffee> {
public:
    void brew()      { std::cout << "brew the grounds\n"; }
    void addExtras() { std::cout << "add sugar and milk\n"; }
};

int main() {
    std::cout << "-- tea --\n";
    Tea tea;
    tea.prepare();
    std::cout << "-- coffee --\n";
    Coffee coffee;
    coffee.prepare();
    return 0;
}
