#include <iostream>
#include <new>
#include <cstdint>

// Over-aligned type: it asks for 64-byte alignment, e.g. a cache line.
struct alignas(64) CacheLine {
    int data[16];
};

int main() {
    std::cout << "alignof(CacheLine) = " << alignof(CacheLine) << "\n";

    // Since C++17, a `new` expression for an over-aligned type automatically
    // calls operator new(size, align_val_t) -- the pointer is 64-byte aligned.
    CacheLine* p = new CacheLine;
    auto addr = reinterpret_cast<std::uintptr_t>(p);
    std::cout << "pointer is 64-byte aligned: "
              << std::boolalpha << (addr % 64 == 0) << "\n";
    delete p;

    // You can also call the alignment-aware operator new/delete by hand.
    // The align_val_t argument must match on both sides.
    void* raw = ::operator new(sizeof(CacheLine), std::align_val_t{64});
    ::operator delete(raw, std::align_val_t{64});
    std::cout << "manual aligned allocation ok\n";
}
