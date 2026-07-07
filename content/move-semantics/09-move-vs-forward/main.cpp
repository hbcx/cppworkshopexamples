#include <iostream>
#include <string>
#include <utility>

struct Gadget {
    std::string id;
    explicit Gadget(std::string s) : id(std::move(s)) {}
    Gadget(const Gadget& o) : id(o.id) { std::cout << "copy\n"; }
    Gadget(Gadget&& o) noexcept : id(std::move(o.id)) { std::cout << "move\n"; }
};

void sink(Gadget) {}   // by value -> constructing its parameter shows copy vs move

// A named parameter of type Gadget&& is itself an LVALUE (it has a name and an
// address). Passing it on directly copies; std::move casts it back to an rvalue.
// std::move is an UNCONDITIONAL cast to rvalue.
void takesRvalueRef(Gadget&& g) {
    sink(g);              // g is a named variable -> lvalue -> copy
    sink(std::move(g));   // cast back to rvalue -> move
}

// In a deduced T&& parameter, std::forward is a CONDITIONAL cast: it restores
// whatever value category the caller used. lvalue in -> lvalue out (copy);
// rvalue in -> rvalue out (move). This is reference collapsing at work.
template <typename T>
void relay(T&& g) {
    sink(std::forward<T>(g));
}

int main() {
    std::cout << "--- named rvalue reference is an lvalue ---\n";
    takesRvalueRef(Gadget("x"));   // copy, then move

    std::cout << "--- std::forward preserves the caller's value category ---\n";
    Gadget lv("lv");
    std::cout << "relay(lvalue): ";
    relay(lv);                     // T = Gadget& -> forwards as lvalue -> copy
    std::cout << "relay(rvalue): ";
    relay(Gadget("rv"));           // T = Gadget -> forwards as rvalue -> move

    std::cout << "--- end ---\n";
    return 0;
}
