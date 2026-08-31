// The smallest allocator a container accepts: value_type, allocate, deallocate,
// plus operator== / operator!=. allocator_traits supplies everything else. This
// one counts what a std::vector asks for.

#include <cstddef>
#include <iostream>
#include <new>
#include <vector>

std::size_t g_allocations = 0;
std::size_t g_bytes = 0;

template <class T>
struct CountingAlloc {
    using value_type = T;

    CountingAlloc() = default;
    template <class U>
    CountingAlloc(const CountingAlloc<U>&) {}   // converting constructor

    T* allocate(std::size_t n) {
        ++g_allocations;
        g_bytes += n * sizeof(T);
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    void deallocate(T* p, std::size_t) { ::operator delete(p); }
};

template <class T, class U>
bool operator==(const CountingAlloc<T>&, const CountingAlloc<U>&) { return true; }
template <class T, class U>
bool operator!=(const CountingAlloc<T>&, const CountingAlloc<U>&) { return false; }

int main() {
    std::vector<int, CountingAlloc<int>> v;
    v.reserve(100);                    // one allocation of 100 ints
    for (int i = 0; i < 10; ++i) v.push_back(i);

    std::cout << "allocations: " << g_allocations << '\n';
    std::cout << "bytes:       " << g_bytes << '\n';
    std::cout << "vector size: " << v.size()
              << ", capacity: " << v.capacity() << '\n';
}
