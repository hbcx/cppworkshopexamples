// Since C++17, new of an over-aligned type honors the alignment: the compiler
// picks the operator new overload that takes std::align_val_t. Before C++17,
// plain new only promised max_align_t, so this object could have been
// under-aligned.

#include <cstdint>
#include <iostream>

struct alignas(32) Avx {
    float v[8];   // 32 bytes of data, asks for a 32-byte boundary
};

static bool aligned32(const void* p) {
    return reinterpret_cast<std::uintptr_t>(p) % 32 == 0;
}

int main() {
    std::cout << "alignof(Avx) = " << alignof(Avx) << '\n';

    Avx* one = new Avx;
    std::cout << "new Avx on a 32-byte boundary? "
              << (aligned32(one) ? "yes" : "no") << '\n';
    delete one;

    Avx* many = new Avx[3];
    std::cout << "new Avx[3] on a 32-byte boundary? "
              << (aligned32(many) ? "yes" : "no") << '\n';
    delete[] many;
}
