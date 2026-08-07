#include <iostream>

// Access control and virtual dispatch are independent. A private virtual can be
// OVERRIDDEN by a derived class even though the derived class cannot NAME it.
class Base {
public:
    virtual ~Base() = default;
    void run() const {
        std::cout << "Base::run calls step -> ";
        step();               // a member may name step(); dispatch picks the override
    }
private:
    virtual void step() const { std::cout << "Base::step\n"; }
};

class Derived : public Base {
private:
    // Overrides a PRIVATE virtual of the base. Access decides who may name
    // step(), not which override runs -- so Base::run() calls this one.
    void step() const override { std::cout << "Derived::step\n"; }
};

int main() {
    Base b;
    b.run();                  // Base::step

    Derived d;
    d.run();                  // Derived::step -- the private virtual was overridden

    // Access and dispatch are independent:
    //   d.step();            // ERROR: step() is private -- cannot be named here
    // Yet Base::run(), a member, calls it and gets Derived::step. That split is
    // exactly what makes NVI safe: the base owns the call, the derived only
    // supplies the behaviour.
    return 0;
}
