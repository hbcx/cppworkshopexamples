#include <iostream>
#include <memory>
#include <string>

struct Target {
    virtual std::string greet() const = 0;
    virtual ~Target() = default;
};

struct Adaptee {
    std::string name;
    std::string hello() const { return "hello " + name; }
};

// Anti-pattern: the adapter stores a REFERENCE to an adaptee. If the adaptee is
// a local (or temporary) that dies before the adapter is used, the reference
// dangles and greet() reads freed memory -- undefined behaviour:
//
//   Target& makeBad() {
//       Adaptee local{"world"};        // dies when makeBad returns
//       static RefAdapter a{local};
//       return a;                      // a.adaptee_ now dangles -- UB on use
//   }
//
// We do NOT run that -- dereferencing a dangling reference is undefined and may
// crash or differ between compilers. It is shown only to name the trap.
struct RefAdapter : Target {
    const Adaptee& adaptee_;
    explicit RefAdapter(const Adaptee& a) : adaptee_(a) {}
    std::string greet() const override { return adaptee_.hello(); }
};

// Fix: make ownership explicit. When the adapter must outlive the caller's
// scope, it OWNS the adaptee (via unique_ptr), so the adaptee lives exactly as
// long as the adapter. (A reference is fine only when the adaptee provably
// outlives the adapter -- and then that contract must be intended.)
class OwningAdapter : public Target {
public:
    explicit OwningAdapter(std::unique_ptr<Adaptee> a) : adaptee_(std::move(a)) {}
    std::string greet() const override { return adaptee_->hello(); }
private:
    std::unique_ptr<Adaptee> adaptee_;
};

std::unique_ptr<Target> makeGreeter() {
    // The adapter owns its adaptee, so returning it out of this scope is safe --
    // nothing dangles.
    return std::make_unique<OwningAdapter>(std::make_unique<Adaptee>(Adaptee{"world"}));
}

int main() {
    std::unique_ptr<Target> g = makeGreeter();
    std::cout << g->greet() << '\n';
    return 0;
}
