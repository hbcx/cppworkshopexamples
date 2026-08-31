// operator== says whether one allocator can free another's memory. The
// propagate_on_container_* traits say whether the allocator itself travels with
// the elements on copy, move and swap. Here swap is made always-valid by
// setting propagate_on_container_swap to true_type.

#include <cstddef>
#include <iostream>
#include <memory>
#include <new>
#include <vector>

template <class T>
struct PoolAlloc {
    using value_type = T;

    // Carried along on assignment and swap instead of staying put.
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap            = std::true_type;

    int id;
    PoolAlloc(int i) : id(i) {}
    template <class U>
    PoolAlloc(const PoolAlloc<U>& o) : id(o.id) {}

    T* allocate(std::size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T* p, std::size_t) { ::operator delete(p); }
};

// Equal only when backed by the same pool.
template <class T, class U>
bool operator==(const PoolAlloc<T>& a, const PoolAlloc<U>& b) { return a.id == b.id; }
template <class T, class U>
bool operator!=(const PoolAlloc<T>& a, const PoolAlloc<U>& b) { return !(a == b); }

int main() {
    using Traits = std::allocator_traits<PoolAlloc<int>>;
    std::cout << std::boolalpha;
    std::cout << "propagate_on_container_swap: "
              << Traits::propagate_on_container_swap::value << '\n';

    std::cout << "pool 1 == pool 1? " << (PoolAlloc<int>(1) == PoolAlloc<int>(1)) << '\n';
    std::cout << "pool 1 == pool 2? " << (PoolAlloc<int>(1) == PoolAlloc<int>(2)) << '\n';

    std::vector<int, PoolAlloc<int>> a({1, 2, 3}, PoolAlloc<int>(1));
    std::vector<int, PoolAlloc<int>> b({9},       PoolAlloc<int>(2));

    a.swap(b);   // valid because the swap trait propagates the allocators too

    std::cout << "after swap: a.size()=" << a.size()
              << " b.size()=" << b.size() << '\n';
}
