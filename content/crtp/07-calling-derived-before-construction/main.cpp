// Anti-pattern: a CRTP base calling into the derived part from its constructor.
//
// The base runs before the derived members exist. If the base constructor calls
// self().configure() and configure() reads a derived field, it reads an
// uninitialized member -- undefined behaviour. We describe it; we do NOT run it.
//
// The fix: do derived-dependent setup AFTER the object is fully built -- an
// explicit init(), or a factory that constructs then configures.

#include <iostream>
#include <string>

// ---------- WRONG: base constructor reaches into the derived part ----------
namespace bad {

template <class Derived>
struct Service {
    Service() {
        // At this point the Derived members are NOT constructed yet.
        //     static_cast<Derived*>(this)->configure();  // reads uninitialized
        //                                                 // field -> UB
        // Left un-called so nothing undefined runs.
    }
};

struct Logger : Service<Logger> {
    std::string prefix = "[log] ";     // constructed AFTER Service<Logger>()
    void configure() { std::cout << prefix << "configured\n"; }
};

} // namespace bad

// ---------- RIGHT: configure after the whole object exists ----------
namespace good {

template <class Derived>
struct Service {
    Service() {} // base constructor touches nothing in the derived part

    Derived& self() { return static_cast<Derived&>(*this); }
};

struct Logger : Service<Logger> {
    std::string prefix = "[log] ";
    void configure() { std::cout << prefix << "configured\n"; }
};

// Factory: construct fully, THEN call the derived setup.
template <class T>
T makeConfigured() {
    T obj;             // fully constructed here
    obj.configure();   // safe: all members alive
    return obj;
}

} // namespace good

int main() {
    bad::Logger legacy;  // safe only because the base ctor does not call configure()
    std::cout << "[bad]  constructed without touching the derived part\n";

    good::Logger logger = good::makeConfigured<good::Logger>();
    (void) logger;
}
