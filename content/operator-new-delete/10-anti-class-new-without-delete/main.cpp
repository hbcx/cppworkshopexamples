#include <iostream>
#include <new>
#include <cstddef>

// BAD: a custom operator new, but NO matching operator delete.
struct Bad {
    static int liveFromPool;
    static void* operator new(std::size_t n) {
        ++liveFromPool;              // our allocator hands one out
        return ::operator new(n);
    }
    // No operator delete -> `delete` falls back to the GLOBAL one, which never
    // tells our allocator the object came back. liveFromPool never drops.
};
int Bad::liveFromPool = 0;

// GOOD: the matching operator delete keeps the bookkeeping balanced.
struct Good {
    static int liveFromPool;
    static void* operator new(std::size_t n) {
        ++liveFromPool;
        return ::operator new(n);
    }
    static void operator delete(void* p) noexcept {
        --liveFromPool;              // our allocator is told it is back
        ::operator delete(p);
    }
};
int Good::liveFromPool = 0;

int main() {
    Bad* a = new Bad;
    delete a;     // global operator delete -- Bad's counter is not touched
    std::cout << "Bad::liveFromPool  after new+delete: "
              << Bad::liveFromPool << " (should be 0)\n";

    Good* b = new Good;
    delete b;     // Good::operator delete -- counter balanced
    std::cout << "Good::liveFromPool after new+delete: "
              << Good::liveFromPool << " (should be 0)\n";
}
