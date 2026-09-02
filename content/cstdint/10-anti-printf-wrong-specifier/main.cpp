#include <cstdint>
#include <cinttypes>
#include <cstdio>

// TRAP: printf("%d", x) where x is an int64_t. "%d" tells printf to pull an int
// (32 bits) off the argument list, but a 64-bit value was pushed. printf reads
// the wrong number of bytes -- wrong result, and every argument after it shifts
// too. It is undefined behaviour, and the compiler's -Wformat refuses to build
// it under -Werror. So the wrong calls are shown only as comments; we run the
// correct ones.
int main() {
    std::int64_t  big  = 9000000000;
    std::uint64_t mask = UINT64_C(0xFFFF0000FFFF0000);

    // WRONG -- do not write these (each is UB, and will not compile under -Werror):
    //     printf("%d\n",  big);    // reads 32 bits of a 64-bit value
    //     printf("%ld\n", big);    // "long" is only 32 bits on Windows

    // RIGHT: the PRI* macros expand to the exact length modifier for the type.
    std::printf("value : %" PRId64 "\n", big);
    std::printf("mask  : 0x%" PRIX64 "\n", mask);
    return 0;
}
