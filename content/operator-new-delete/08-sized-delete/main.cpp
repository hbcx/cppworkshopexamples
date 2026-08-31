#include <iostream>
#include <new>
#include <cstddef>

struct Blob {
    int payload[8];

    static void* operator new(std::size_t n) {
        return ::operator new(n);
    }
    // Sized deallocation (C++14): the compiler passes the object's size, so a
    // pool allocator can route to the right free-list without storing the size
    // itself. If both this and the plain operator delete(void*) exist, the
    // compiler prefers this sized form.
    static void operator delete(void* p, std::size_t n) noexcept {
        std::cout << "sized delete: freeing " << n << " bytes\n";
        ::operator delete(p);
    }
};

int main() {
    Blob* b = new Blob;
    delete b;     // calls Blob::operator delete(void*, std::size_t)
}
