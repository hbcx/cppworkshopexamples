#include <cstddef>
#include <cstring>
#include <iostream>
#include <type_traits>

int elementCopies = 0;

// A non-trivially-copyable type: its copy assignment counts calls, so we can
// see when the element-by-element path runs.
struct Counted {
    int v;
    Counted() : v(0) {}
    Counted(int x) : v(x) {}
    Counted(const Counted&) = default;
    Counted& operator=(const Counted& o) { v = o.v; ++elementCopies; return *this; }
};

// Fast path: bulk-copy the raw bytes. Only ever instantiated for trivially
// copyable types, so it never clobbers an object that needs its copy assignment.
template <class T>
void copyRange(const T* src, T* dst, std::size_t n, std::true_type) {
    std::memcpy(dst, src, n * sizeof(T));
    std::cout << "memcpy path (" << n << " objects)\n";
}
// Safe path: element by element, so each object's copy assignment runs.
template <class T>
void copyRange(const T* src, T* dst, std::size_t n, std::false_type) {
    for (std::size_t i = 0; i < n; ++i) dst[i] = src[i];
    std::cout << "element path (" << n << " objects)\n";
}
// Dispatcher: build the tag from the trait and forward it.
template <class T>
void copyRange(const T* src, T* dst, std::size_t n) {
    copyRange(src, dst, n, std::is_trivially_copyable<T>{});
}

int main() {
    int a[3] = {1, 2, 3};
    int b[3] = {0, 0, 0};
    copyRange(a, b, 3);                       // int is trivially copyable -> memcpy
    std::cout << "b = " << b[0] << " " << b[1] << " " << b[2] << "\n";

    Counted c[3] = { Counted(10), Counted(20), Counted(30) };
    Counted d[3];
    copyRange(c, d, 3);                       // Counted has a user copy assign -> element path
    std::cout << "d = " << d[0].v << " " << d[1].v << " " << d[2].v << "\n";
    std::cout << "elementCopies = " << elementCopies << "\n";
    return 0;
}
