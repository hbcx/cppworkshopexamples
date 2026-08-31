// Anti-pattern: a custom allocator whose allocate ignores the type's alignment.
//
// Plain operator new only promises max_align_t (16). For an over-aligned element
// type the container then holds an under-aligned buffer -- correct-looking,
// latent, and a fault on strict CPUs. Allocate with std::align_val_t instead.

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <new>
#include <vector>

struct alignas(32) Avx {
    float v[8];
};

template <class T>
struct AlignedAlloc {
    using value_type = T;

    AlignedAlloc() = default;
    template <class U>
    AlignedAlloc(const AlignedAlloc<U>&) {}

    T* allocate(std::size_t n) {
        // BAD (only max_align_t -- described, not used):
        //   return static_cast<T*>(::operator new(n * sizeof(T)));
        // For an alignas(32) T that buffer may be only 16-aligned.

        // GOOD: ask for the type's alignment.
        return static_cast<T*>(
            ::operator new(n * sizeof(T), std::align_val_t{alignof(T)}));
    }
    void deallocate(T* p, std::size_t) {
        ::operator delete(p, std::align_val_t{alignof(T)});
    }
};

template <class T, class U>
bool operator==(const AlignedAlloc<T>&, const AlignedAlloc<U>&) { return true; }
template <class T, class U>
bool operator!=(const AlignedAlloc<T>&, const AlignedAlloc<U>&) { return false; }

int main() {
    std::vector<Avx, AlignedAlloc<Avx>> v;
    v.reserve(4);
    v.push_back(Avx{});
    v.push_back(Avx{});

    bool ok = reinterpret_cast<std::uintptr_t>(v.data()) % 32 == 0;
    std::cout << "alignof(Avx) = " << alignof(Avx) << '\n';
    std::cout << "vector buffer on a 32-byte boundary? "
              << std::boolalpha << ok << '\n';
}
