#include <forward_list>
#include <list>
#include <iostream>
#include <cstddef>
#include <new>

// A minimal allocator that just counts the bytes requested. A node-based
// container asks it for exactly one node per element, so total / count is the
// real size of one node -- the honest way to compare the two layouts.
static std::size_t g_bytes = 0;

template <typename T>
struct CountingAlloc {
    using value_type = T;
    CountingAlloc() = default;
    template <typename U> CountingAlloc(const CountingAlloc<U>&) {}
    T* allocate(std::size_t n) {
        g_bytes += n * sizeof(T);
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    void deallocate(T* p, std::size_t) { ::operator delete(p); }
    template <typename U> bool operator==(const CountingAlloc<U>&) const { return true; }
    template <typename U> bool operator!=(const CountingAlloc<U>&) const { return false; }
};

int main() {
    const int N = 1000;

    g_bytes = 0;
    {
        std::forward_list<int, CountingAlloc<int>> fl;
        for (int i = 0; i < N; ++i) fl.push_front(i);
    }
    std::size_t fwd = g_bytes / N;

    g_bytes = 0;
    {
        std::list<int, CountingAlloc<int>> ls;
        for (int i = 0; i < N; ++i) ls.push_front(i);
    }
    std::size_t lst = g_bytes / N;

    std::cout << "element:             sizeof(int) = " << sizeof(int) << " bytes\n";
    std::cout << "forward_list node:   " << fwd << " bytes (one next pointer)\n";
    std::cout << "list node:           " << lst << " bytes (next + prev)\n";
    std::cout << "saved per node:      " << (lst - fwd) << " bytes\n";
    return 0;
}
