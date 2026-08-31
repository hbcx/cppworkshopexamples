// std::max_align_t is the strictest alignment plain new and malloc guarantee.
// Their memory is aligned for that worst-case type, so it fits any ordinary
// type -- and that same value is the ceiling an over-aligned type exceeds.

#include <cstddef>   // std::max_align_t
#include <cstdint>
#include <iostream>

int main() {
    const std::size_t guaranteed = alignof(std::max_align_t);
    std::cout << "alignof(std::max_align_t) = " << guaranteed << '\n';

    // A default allocation is aligned to at least that boundary.
    double* p = new double(3.5);
    bool ok = reinterpret_cast<std::uintptr_t>(p) % guaranteed == 0;
    std::cout << "new double meets the max_align_t boundary? "
              << (ok ? "yes" : "no") << '\n';
    delete p;

    std::cout << "so any type needing <= " << guaranteed
              << "-byte alignment is safe with plain new;\n"
              << "a type needing more is over-aligned (see over-aligned new)\n";
}
