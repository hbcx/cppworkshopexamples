#include <iostream>
#include <string>

// const means "cannot be modified through this name". Its most common and most
// valuable use is a read-only reference parameter: const T& binds to an lvalue
// OR a temporary and copies nothing, so it is the default way to pass something
// you only read.

// Reads its argument; const& promises not to modify it and avoids a copy.
std::size_t lengthOf(const std::string& s) {
    // s += "!";   // would not compile: s is const here
    return s.size();
}

int main() {
    const int limit = 100;      // a constant object
    // limit = 200;             // error: assignment of read-only variable
    std::cout << "limit = " << limit << "\n";

    std::string owned = "hello world";

    // One function, called with an lvalue and with a temporary -- both bind to
    // const&, and neither is copied.
    std::cout << "length of lvalue:    " << lengthOf(owned) << "\n";
    std::cout << "length of temporary: " << lengthOf(std::string(5, 'x')) << "\n";

    // A const reference is a read-only view of an existing object -- no copy,
    // and you cannot change the object through it.
    const std::string& view = owned;
    // view += "!";             // error: view refers to const
    std::cout << "view = " << view << " (aliases owned, read-only)\n";

    // Marking a value you keep const documents intent and lets the compiler
    // catch an accidental write.
    const std::size_t half = owned.size() / 2;
    std::cout << "half = " << half << "\n";
    return 0;
}
