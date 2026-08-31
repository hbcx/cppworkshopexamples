// Anti-pattern: assuming a copied pmr container keeps the original's resource.
//
// polymorphic_allocator does NOT propagate on copy. auto b = a gives b the
// default (heap) resource, not a's stack buffer. To keep the resource you must
// pass it to the copy explicitly.

#include <array>
#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <vector>

int main() {
    std::array<std::byte, 512> buffer;
    std::pmr::monotonic_buffer_resource res{buffer.data(), buffer.size()};

    std::pmr::vector<int> a{&res};
    a.reserve(8);
    for (int i = 0; i < 8; ++i) a.push_back(i);

    // BAD: a plain copy silently uses the DEFAULT resource, not res.
    std::pmr::vector<int> bad = a;
    bool badOnRes = bad.get_allocator().resource() == &res;
    std::cout << std::boolalpha;
    std::cout << "plain copy uses the stack buffer? " << badOnRes
              << "  (it fell back to the heap)\n";

    // GOOD: pass the resource so the copy stays on the buffer.
    std::pmr::vector<int> good{a, &res};
    bool goodOnRes = good.get_allocator().resource() == &res;
    std::cout << "explicit-resource copy uses the stack buffer? " << goodOnRes << '\n';
}
