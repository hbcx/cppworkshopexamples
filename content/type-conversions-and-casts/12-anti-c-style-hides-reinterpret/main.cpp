#include <iostream>

struct Widget { int id; };
struct Gadget { double value; };   // completely unrelated to Widget

int main() {
    Widget w{7};

    // BAD: a C-style cast between unrelated pointer types compiles, silently doing
    // a reinterpret_cast. The pointer is non-null, but reading gadget->value would
    // be undefined behavior. We do NOT dereference it.
    Gadget* gadget = (Gadget*)&w;
    std::cout << "C-style cast compiled, pointer non-null? " << (gadget != nullptr)
              << "  (using it would be undefined behavior)\n";     // 1

    // GOOD: the named cast refuses the same conversion, so the mistake is caught
    // at compile time:
    //   Gadget* g = static_cast<Gadget*>(&w);   // ERROR: unrelated types
    // If reinterpretation is truly intended, reinterpret_cast SAYS so and puts the
    // responsibility in plain sight.

    std::cout << "widget id is still just " << w.id << "\n";        // 7
    return 0;
}
