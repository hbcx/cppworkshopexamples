// Anti-pattern: treating allocator storage as zero-initialized.
//
// allocate is not calloc -- it returns raw bytes holding whatever was there.
// Reading a slot before construct is undefined. To get a defined zero, construct
// the objects; value-construction (the {} form) zeroes trivial types.

#include <iostream>
#include <memory>   // std::allocator, std::allocator_traits

int main() {
    std::allocator<int> a;
    using Traits = std::allocator_traits<std::allocator<int>>;

    const std::size_t n = 4;
    int* p = Traits::allocate(a, n);

    // BAD (indeterminate -- described, not read):
    //   for (std::size_t i = 0; i < n; ++i) std::cout << p[i];  // junk, and UB

    // GOOD: value-construct to get guaranteed zeros.
    for (std::size_t i = 0; i < n; ++i) Traits::construct(a, p + i);  // int{} == 0

    std::cout << "value-constructed:";
    for (std::size_t i = 0; i < n; ++i) std::cout << ' ' << p[i];
    std::cout << '\n';

    for (std::size_t i = 0; i < n; ++i) Traits::destroy(a, p + i);
    Traits::deallocate(a, p, n);
}
