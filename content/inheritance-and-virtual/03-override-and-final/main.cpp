#include <iostream>

class Base {
public:
    virtual const char* kind() const { return "Base"; }
    virtual ~Base() = default;
};

class Middle : public Base {
public:
    // override: the compiler confirms this matches Base::kind. A typo here
    // (say, kind() without const) would be a compile error, not a silent new
    // function.
    const char* kind() const override { return "Middle"; }
};

class Leaf final : public Middle {   // final class: nothing may inherit from Leaf
public:
    // final on the function: no further class could override kind() even if it
    // could derive from Leaf.
    const char* kind() const override final { return "Leaf"; }
};

int main() {
    Leaf leaf;
    Base& b = leaf;
    std::cout << "kind: " << b.kind() << "\n";   // Leaf   (virtual dispatch)

    // A class derived from Leaf, or a further override of kind(), would now fail
    // to compile -- final has sealed both.
    return 0;
}
