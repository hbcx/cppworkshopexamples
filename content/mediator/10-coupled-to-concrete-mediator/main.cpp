// Anti-pattern: a colleague coupled to the concrete mediator.
//
// If a colleague holds a pointer to the CONCRETE mediator type, it is welded to
// that one mediator: it cannot be reused in another context, driven by a
// different coordination, or tested with a stub. Depending on a Mediator
// INTERFACE instead makes the colleague reusable with any mediator -- the real
// one in production, a logging or test double in a test. The good field below is
// driven by two different mediators without changing the field at all.

#include <iostream>
#include <string>

// ---- BAD: the field is bound to one concrete mediator ----
class ConcreteHub {
public:
    void changed(const std::string& field) { std::cout << "  hub saw " << field << " change\n"; }
};
class FieldBad {
    ConcreteHub* hub_; // only a ConcreteHub will ever do
public:
    explicit FieldBad(ConcreteHub* h) : hub_(h) {}
    void edit() { hub_->changed("bad-field"); }
};

// ---- GOOD: the field depends on the Mediator interface ----
class Mediator {
public:
    virtual ~Mediator() = default;
    virtual void changed(const std::string& field) = 0;
};
class FieldGood {
    Mediator* mediator_; // any mediator will do
public:
    explicit FieldGood(Mediator* m) : mediator_(m) {}
    void edit() { mediator_->changed("good-field"); }
};

class RealHub : public Mediator {
public:
    void changed(const std::string& field) override { std::cout << "  real hub: " << field << "\n"; }
};
class TestHub : public Mediator { // a test double, usable because Field depends on the interface
public:
    void changed(const std::string& field) override { std::cout << "  test double logged: " << field << "\n"; }
};

int main() {
    std::cout << "-- bad (welded to ConcreteHub) --\n";
    ConcreteHub hub;
    FieldBad bad(&hub);
    bad.edit();

    std::cout << "-- good (same field, two mediators) --\n";
    RealHub real;
    TestHub test;
    FieldGood inProd(&real); // same class...
    FieldGood inTest(&test); // ...driven by a different mediator
    inProd.edit();
    inTest.edit();
    return 0;
}
