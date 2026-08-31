// Anti-pattern: a bump allocator that returns base + offset with no rounding.
//
// After a 1-byte allocation the offset is odd, so the next double comes back on
// an odd address -- misaligned. The fix is to round the offset up to the
// requested alignment before handing out the pointer; std::align does it.

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>   // std::align

struct Bump {
    alignas(std::max_align_t) unsigned char pool[256];
    std::size_t used = 0;

    // BAD: no alignment rounding.
    void* bad_alloc(std::size_t n) {
        void* p = pool + used;
        used += n;
        return p;
    }

    // GOOD: round up to `al` first with std::align.
    void* good_alloc(std::size_t n, std::size_t al) {
        void* p = pool + used;
        std::size_t space = sizeof(pool) - used;
        void* a = std::align(al, n, p, space);
        used = (static_cast<unsigned char*>(a) - pool) + n;
        return a;
    }

    std::size_t offset_of(void* p) const {
        return static_cast<unsigned char*>(p) - pool;
    }
};

int main() {
    {
        Bump b;
        b.bad_alloc(1);                     // one byte -> offset now 1
        void* d = b.bad_alloc(sizeof(double));
        std::cout << "bad_alloc: double at offset " << b.offset_of(d)
                  << ", 8-aligned? "
                  << (b.offset_of(d) % 8 == 0 ? "yes" : "no") << '\n';
    }
    {
        Bump b;
        b.good_alloc(1, 1);                 // one byte -> offset now 1
        void* d = b.good_alloc(sizeof(double), alignof(double));
        std::cout << "good_alloc: double at offset " << b.offset_of(d)
                  << ", 8-aligned? "
                  << (b.offset_of(d) % 8 == 0 ? "yes" : "no") << '\n';
    }
}
