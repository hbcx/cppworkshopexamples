#include <iostream>
#include <new>
#include <cstdint>

struct alignas(32) Vec8 {     // e.g. an AVX register: needs 32-byte alignment
    float v[8];
};

int main() {
    std::cout << "Vec8 needs alignment " << alignof(Vec8) << "\n";

    // WRONG (undefined behavior, NOT run here):
    //   unsigned char raw[sizeof(Vec8)];   // a char buffer has alignment 1
    //   Vec8* p = new (raw) Vec8;           // object built under-aligned
    //   p->v[0] = 1.0f;                     // misaligned access -> UB
    // A plain char buffer, malloc, or ::operator new(size) does NOT promise
    // 32-byte alignment; only alignas storage or an aligned allocation does.

    // RIGHT #1: alignas gives the buffer the required alignment.
    alignas(Vec8) unsigned char buf[sizeof(Vec8)];
    Vec8* a = new (buf) Vec8;
    auto addrA = reinterpret_cast<std::uintptr_t>(a);
    std::cout << "alignas buffer aligned: "
              << std::boolalpha << (addrA % alignof(Vec8) == 0) << "\n";
    a->v[0] = 1.0f;
    a->~Vec8();

    // RIGHT #2: the alignment-aware operator new (C++17).
    void* mem = ::operator new(sizeof(Vec8), std::align_val_t{alignof(Vec8)});
    Vec8* b = new (mem) Vec8;
    auto addrB = reinterpret_cast<std::uintptr_t>(b);
    std::cout << "aligned operator new aligned: "
              << (addrB % alignof(Vec8) == 0) << "\n";
    b->~Vec8();
    ::operator delete(mem, std::align_val_t{alignof(Vec8)});
}
