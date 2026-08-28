#include <iostream>

// Anti-pattern: intending to override, but the signature does not match. The base
// describe() is const; the derived one forgets const, so it is a new function, not
// an override -- and a call through const Base& runs the base version.

class Base {
public:
    virtual void describe() const { std::cout << "Base\n"; }
    virtual ~Base() = default;
};

class Broken : public Base {
public:
    // Meant to override, but MISSING const -> a different signature -> NOT an
    // override. Writing `override` here would make this a compile error.
    void describe() { std::cout << "Broken\n"; }
};

class Fixed : public Base {
public:
    // Exact match + override keyword: a real override, checked by the compiler.
    void describe() const override { std::cout << "Fixed\n"; }
};

// Polymorphic call site: takes the base by const reference.
void report(const Base& b) { b.describe(); }

int main() {
    Broken broken;
    Fixed  fixed;

    report(broken);   // Base   <-- the override was never hooked up (the bug)
    report(fixed);    // Fixed  <-- correct
    return 0;
}
