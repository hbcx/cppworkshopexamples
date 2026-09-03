#include <functional>
#include <iostream>
#include <string>

std::string makeName() { return "generated"; }

int main() {
    // ANTI-PATTERN: std::ref on a temporary. std::ref takes an lvalue reference;
    // the standard DELETES the rvalue overload (std::ref(T&&)), so wrapping a
    // temporary is a COMPILE error -- the library refuses to build a wrapper that
    // would instantly dangle:
    //
    //   auto bad  = std::ref(makeName());          // ref(string&&) is deleted
    //   auto bad2 = std::ref(std::string("x"));    // same -- rvalue rejected
    //
    // That deletion is a feature: a wrapper to a temporary would refer to an
    // object destroyed at the end of the full expression.

    // FIX: bind the temporary to a NAMED variable first, so there is a real
    // object with a lifetime to refer to.
    std::string name = makeName();     // the object now lives in `name`
    auto good = std::ref(name);
    std::cout << "good -> " << good.get() << "\n";   // generated
    name += "!";
    std::cout << "good -> " << good.get() << "\n";   // generated!
    return 0;
}
