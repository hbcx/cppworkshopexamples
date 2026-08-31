// Anti-pattern: using an allocator's storage before constructing objects in it,
// or deallocating without destroying first.
//
// allocate/deallocate move bytes; construct/destroy run the lifetime. They are
// separate steps -- skipping construct reads a non-object, skipping destroy
// leaks whatever the elements own.

#include <iostream>
#include <memory>   // std::allocator, std::allocator_traits
#include <string>

int main() {
    std::allocator<std::string> a;
    using Traits = std::allocator_traits<std::allocator<std::string>>;

    std::string* p = Traits::allocate(a, 2);   // raw bytes, no strings yet

    // BAD (undefined -- described, not run):
    //   std::cout << p[0];        // reading a string that was never constructed
    //   Traits::deallocate(a, p, 2);  // freeing without destroy leaks each
    //                                 // string's heap buffer

    // GOOD: construct, use, destroy, deallocate.
    Traits::construct(a, p + 0, "constructed first");
    Traits::construct(a, p + 1, "constructed second");

    std::cout << p[0] << '\n' << p[1] << '\n';

    Traits::destroy(a, p + 0);
    Traits::destroy(a, p + 1);
    Traits::deallocate(a, p, 2);
    std::cout << "each allocate paired with construct, each destroy with deallocate\n";
}
