// False sharing: a CPU moves memory one cache line at a time (64 bytes here).
// Two counters on the same line, written by two threads, make the line bounce
// between cores. alignas(64) puts each on its own line. We prove the layout
// without threads -- the fields land 64 bytes apart, so the struct is 128 bytes.

#include <cstddef>   // offsetof
#include <cstdint>   // std::int64_t
#include <iostream>

struct Packed {
    std::int64_t a;
    std::int64_t b;   // shares a cache line with a: false-sharing risk
};

struct Separated {
    alignas(64) std::int64_t a;
    alignas(64) std::int64_t b;   // forced onto its own cache line
};

int main() {
    std::cout << "sizeof(Packed)    = " << sizeof(Packed)    << '\n';
    std::cout << "sizeof(Separated) = " << sizeof(Separated) << '\n';

    std::size_t gap = offsetof(Separated, b) - offsetof(Separated, a);
    std::cout << "a and b are " << gap << " bytes apart\n";
    std::cout << "on different 64-byte lines? "
              << (gap >= 64 ? "yes" : "no") << '\n';
}
