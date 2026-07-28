#include <iostream>
#include <memory>

// Anti-pattern: owning a Derived through unique_ptr<Base> when Base's destructor
// is NOT virtual. Deleting through a base pointer without a virtual destructor
// is undefined behaviour: typically only ~Base runs and the derived part -- and
// anything it owns -- leaks. The FIX is a virtual destructor on the base, which
// we run here; the broken case is described, not executed (it would be UB).
class Base {
public:
    Base() { std::cout << "Base ctor\n"; }
    virtual ~Base() { std::cout << "Base dtor\n"; }   // virtual: this is the fix
    virtual void poll() const = 0;
};

class Derived : public Base {
public:
    Derived() : buffer_(new int[4]()) { std::cout << "Derived ctor\n"; }
    ~Derived() override {
        delete[] buffer_;                             // only runs if the chain is correct
        std::cout << "Derived dtor (buffer freed)\n";
    }
    void poll() const override { std::cout << "Derived polling\n"; }
private:
    int* buffer_;
};

int main() {
    std::unique_ptr<Base> p(new Derived());   // a Base* owning a Derived
    p->poll();

    // With the virtual ~Base above, destroying p runs the full chain,
    // most-derived first:
    p.reset();                                 // Derived dtor THEN Base dtor
    std::cout << "p owns: " << (p ? "yes" : "no") << '\n';

    // Had ~Base been non-virtual, p.reset() (or scope exit) would be undefined
    // behaviour: in practice only "Base dtor" would print, Derived::~Derived
    // would be skipped, and buffer_ would leak. That is why a polymorphic base
    // MUST have a virtual destructor.
    return 0;
}
