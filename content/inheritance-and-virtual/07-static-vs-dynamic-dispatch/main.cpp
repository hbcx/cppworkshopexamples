#include <iostream>

// Base has one non-virtual function and one virtual function. Called through a
// Base&, they resolve differently: the non-virtual by the reference's type, the
// virtual by the real object.

class Base {
public:
    const char* staticName() const { return "Base"; }          // NON-virtual
    virtual const char* dynamicName() const { return "Base"; } // virtual
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    const char* staticName() const { return "Derived"; }             // hides, not overrides
    const char* dynamicName() const override { return "Derived"; }   // overrides
};

int main() {
    Derived d;
    Base& b = d;   // one object, seen through a Base reference

    // Non-virtual: resolved from the STATIC type (Base&) -> "Base".
    std::cout << "staticName():  " << b.staticName()  << "\n";   // Base

    // Virtual: resolved from the REAL object (Derived) -> "Derived".
    std::cout << "dynamicName(): " << b.dynamicName() << "\n";   // Derived

    // Through the Derived handle directly, staticName() picks Derived's:
    std::cout << "d.staticName(): " << d.staticName() << "\n";   // Derived
    return 0;
}
