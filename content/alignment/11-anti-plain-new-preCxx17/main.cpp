// Anti-pattern: trusting plain new to over-align on a pre-C++17 compiler.
//
// Under C++14 and earlier, new of an over-aligned type only promises
// max_align_t, so the object can be under-aligned -- and nothing warns you.
// The portable fix (and what C++17 new calls internally) is the operator new
// overload that takes std::align_val_t, plus placement new.

#include <cstdint>
#include <iostream>
#include <new>   // std::align_val_t, operator new overloads, placement new

struct alignas(32) Avx {
    float v[8];
};

int main() {
    // BAD (silent on a pre-C++17 toolchain -- described, not run):
    //   Avx* p = new Avx;   // under C++14 this may be only 16-aligned
    //   // use p ... on a 16-aligned Avx that promised 32: undefined

    // GOOD: ask for the alignment explicitly, then construct in place.
    void* raw = ::operator new(sizeof(Avx), std::align_val_t{alignof(Avx)});
    Avx* p = new (raw) Avx{};

    bool ok = reinterpret_cast<std::uintptr_t>(p) % 32 == 0;
    std::cout << "explicit aligned allocation on a 32-byte boundary? "
              << (ok ? "yes" : "no") << '\n';

    p->~Avx();
    ::operator delete(raw, std::align_val_t{alignof(Avx)});
}
