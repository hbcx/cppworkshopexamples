#include <iostream>
#include <new>       // std::nothrow

int main() {
    // Ordinary `new` throws std::bad_alloc when it cannot allocate. The
    // nothrow form returns a null pointer instead -- useful when exceptions
    // are turned off, or when you want to handle failure right here.
    int* p = new (std::nothrow) int(123);

    if (p == nullptr) {
        std::cout << "allocation failed\n";   // reached instead of a throw
        return 1;
    }

    std::cout << "value = " << *p << "\n";

    // A completed nothrow-allocated object is released with ordinary delete.
    delete p;
}
