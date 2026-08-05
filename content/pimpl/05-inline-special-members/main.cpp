#include "gadget.hpp"

#include <iostream>
#include <utility>

int main() {
    Gadget g("g-1");
    std::cout << "id = " << g.id() << "\n";

    Gadget h = std::move(g); // uses the out-of-line move constructor
    std::cout << "after move: g=" << g.id() << " h=" << h.id() << "\n";
}
