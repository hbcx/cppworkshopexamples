#include <iostream>
#include <memory>

// Base has a VIRTUAL destructor, so deleting a Derived through a Base pointer runs
// both destructors in the right order. The prints make the order visible.

class Base {
public:
    Base() { std::cout << "Base()\n"; }
    virtual ~Base() { std::cout << "~Base()\n"; }   // virtual: the crucial part
};

class Derived : public Base {
public:
    Derived() { std::cout << "Derived()\n"; }
    ~Derived() override { std::cout << "~Derived()  (frees the resource)\n"; }
};

int main() {
    // Held and deleted through a Base pointer -- the polymorphic case.
    std::unique_ptr<Base> p(new Derived());
    std::cout << "-- destroying through Base* --\n";
    p.reset();   // runs ~Derived() then ~Base(), thanks to the virtual destructor

    // Had ~Base() been non-virtual, only ~Base() would run here -- the Derived
    // cleanup would be skipped and the program would be in undefined behavior.
    return 0;
}
