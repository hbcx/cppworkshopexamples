// Containers reach an allocator through std::allocator_traits, not directly.
// The traits forward allocate/deallocate and provide construct/destroy/max_size
// as defaults, so the same four steps work through one uniform interface.

#include <iostream>
#include <memory>   // std::allocator, std::allocator_traits

int main() {
    std::allocator<int> a;
    using Traits = std::allocator_traits<std::allocator<int>>;

    int* p = Traits::allocate(a, 4);              // raw storage for 4 ints
    for (int i = 0; i < 4; ++i)
        Traits::construct(a, p + i, (i + 1) * 10); // build each int

    std::cout << "constructed:";
    for (int i = 0; i < 4; ++i) std::cout << ' ' << p[i];
    std::cout << '\n';

    for (int i = 0; i < 4; ++i) Traits::destroy(a, p + i);
    Traits::deallocate(a, p, 4);

    // max_size is a default the traits compute, even though std::allocator
    // barely needs to define anything.
    std::cout << "traits report the pointer type is int*: "
              << std::boolalpha
              << std::is_same<Traits::pointer, int*>::value << '\n';
}
