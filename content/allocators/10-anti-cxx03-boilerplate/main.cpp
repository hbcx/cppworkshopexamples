// Anti-pattern: writing the full pre-C++11 allocator interface by hand.
//
// The old way meant pointer/const_pointer/reference/rebind/construct/destroy/
// max_size, each a chance for a wrong signature. Since C++11 allocator_traits
// derives all of them from just value_type + allocate + deallocate.

#include <cstddef>
#include <iostream>
#include <memory>
#include <new>
#include <type_traits>
#include <vector>

// The whole modern allocator: three members plus equality.
template <class T>
struct Minimal {
    using value_type = T;

    Minimal() = default;
    template <class U>
    Minimal(const Minimal<U>&) {}

    T* allocate(std::size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T* p, std::size_t) { ::operator delete(p); }
};

template <class T, class U>
bool operator==(const Minimal<T>&, const Minimal<U>&) { return true; }
template <class T, class U>
bool operator!=(const Minimal<T>&, const Minimal<U>&) { return false; }

int main() {
    using Tr = std::allocator_traits<Minimal<int>>;
    std::cout << std::boolalpha;

    // None of these were declared by Minimal -- the traits supplied them.
    std::cout << "traits::pointer is int*: "
              << std::is_same<Tr::pointer, int*>::value << '\n';
    std::cout << "traits::rebind_alloc<double> is Minimal<double>: "
              << std::is_same<Tr::rebind_alloc<double>, Minimal<double>>::value << '\n';

    // And it just works in a container.
    std::vector<int, Minimal<int>> v;
    v.reserve(8);
    for (int i = 0; i < 5; ++i) v.push_back(i);
    std::cout << "vector with the minimal allocator holds " << v.size()
              << " elements\n";
}
