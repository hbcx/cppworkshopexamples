// Anti-pattern: code bloat from one base copy per CRTP type.
//
// Every CRTP instantiation generates a fresh copy of the base's code. A big base
// mixed into many types is stamped out once per type -- larger binary, slower
// builds, all in the header. Where the calls are cold, a single virtual base is
// smaller and simpler.
//
// This sketches both shapes to make the difference clear; it does not measure
// binary size (that is toolchain-specific).

#include <iostream>
#include <string>

// ---------- CRTP: the (large) base is duplicated per record type ----------
namespace bloat {

template <class Derived>
class Serializer {
public:
    // Imagine this is large and cold. It is generated ONCE PER Derived type.
    std::string serialize() const {
        const Derived& d = static_cast<const Derived&>(*this);
        return "{" + d.fields() + "}";
    }
};

struct User : Serializer<User> {
    std::string fields() const { return "user"; }
};
struct Order : Serializer<Order> {
    std::string fields() const { return "order"; }
};
struct Invoice : Serializer<Invoice> {
    std::string fields() const { return "invoice"; }
};
// serialize() now exists three times over -- one copy per type.

} // namespace bloat

// ---------- virtual: one shared copy of the cold method ----------
namespace shared {

class Serializer {
public:
    virtual ~Serializer() {}
    virtual std::string fields() const = 0;
    // Exists ONCE, shared by every subclass through the vtable.
    std::string serialize() const { return "{" + fields() + "}"; }
};

struct User : Serializer {
    std::string fields() const override { return "user"; }
};
struct Order : Serializer {
    std::string fields() const override { return "order"; }
};

} // namespace shared

int main() {
    bloat::User u;
    bloat::Order o;
    bloat::Invoice i;
    std::cout << "[crtp]    " << u.serialize() << " " << o.serialize()
              << " " << i.serialize() << " (serialize() generated per type)\n";

    shared::User su;
    shared::Order so;
    std::cout << "[virtual] " << su.serialize() << " " << so.serialize()
              << " (one shared serialize())\n";
}
