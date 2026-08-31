// uninitialized_fill: copy-construct one value into every slot of raw storage.
//
// This is what a container does when it grows and fills the fresh capacity with
// a default element: the slots are raw, so each one needs a real constructor
// call, not an assignment.

#include <iostream>
#include <memory>   // std::uninitialized_fill
#include <string>

int main() {
    using String = std::string;
    const std::size_t count = 4;

    // Raw storage for `count` strings -- no objects constructed yet.
    String* buf = static_cast<String*>(::operator new(sizeof(String) * count));

    // Copy-construct the same value into each slot. Every slot gets its own
    // independent string, not a shared one.
    std::uninitialized_fill(buf, buf + count, String("pending"));

    std::cout << "filled " << count << " slots with:\n";
    for (String* p = buf; p != buf + count; ++p) {
        std::cout << "  " << *p << '\n';
    }

    // Each slot is a separate object, so each needs its own destructor.
    for (String* p = buf; p != buf + count; ++p) {
        p->~String();
    }
    ::operator delete(buf);
}
