// An allocator separates getting memory from making objects:
//   allocate / deallocate  -- raw bytes
//   construct / destroy    -- object lifetime (here done by hand)
// This is the split that lets a container hold more capacity than it has
// live elements.

#include <iostream>
#include <memory>   // std::allocator
#include <string>

int main() {
    using Str = std::string;
    std::allocator<Str> a;

    // Step 1: raw storage for 3 strings -- no objects yet.
    Str* p = a.allocate(3);

    // Step 2: turn the bytes into objects with placement new.
    new (p + 0) Str("alpha");
    new (p + 1) Str("beta");
    new (p + 2) Str("gamma");

    for (int i = 0; i < 3; ++i) std::cout << "  " << p[i] << '\n';

    // Step 3: end each object's lifetime with a destructor call.
    for (int i = 0; i < 3; ++i) p[i].~Str();

    // Step 4: give the raw bytes back.
    a.deallocate(p, 3);

    std::cout << "allocate + construct + destroy + deallocate: done\n";
}
