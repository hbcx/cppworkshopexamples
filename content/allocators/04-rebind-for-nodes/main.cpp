// A node-based container allocates NODES, not your element type. std::list<int>
// with your allocator<int> rebinds it to allocator<ListNode> via
// allocator_traits::rebind_alloc. Every allocator must therefore be a template
// usable for any type.

#include <cstddef>
#include <iostream>
#include <list>
#include <memory>
#include <new>
#include <type_traits>

std::size_t g_nodeAllocs = 0;

template <class T>
struct NodeAlloc {
    using value_type = T;

    NodeAlloc() = default;
    template <class U>
    NodeAlloc(const NodeAlloc<U>&) {}

    T* allocate(std::size_t n) {
        ++g_nodeAllocs;   // shared across every rebound type
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    void deallocate(T* p, std::size_t) { ::operator delete(p); }
};

template <class T, class U>
bool operator==(const NodeAlloc<T>&, const NodeAlloc<U>&) { return true; }
template <class T, class U>
bool operator!=(const NodeAlloc<T>&, const NodeAlloc<U>&) { return false; }

int main() {
    // rebind_alloc<double> of NodeAlloc<int> is exactly NodeAlloc<double>.
    using IntAlloc = NodeAlloc<int>;
    using Rebound = std::allocator_traits<IntAlloc>::rebind_alloc<double>;
    std::cout << "rebind_alloc<double> is NodeAlloc<double>: "
              << std::boolalpha
              << std::is_same<Rebound, NodeAlloc<double>>::value << '\n';

    // A list of 3 ints allocates 3 nodes through the rebound allocator.
    std::list<int, NodeAlloc<int>> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    std::cout << "node allocations for 3 elements: " << g_nodeAllocs << '\n';
}
