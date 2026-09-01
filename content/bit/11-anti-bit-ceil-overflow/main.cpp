#include <bit>
#include <cstdint>
#include <iostream>
#include <limits>

int main() {
    // The <bit> functions require an UNSIGNED integer type. A signed argument is
    // a compile error -- a good guard, not a silent bug. These would not compile:
    //     std::bit_ceil(17);         // ERROR: int is signed
    //     std::has_single_bit(-4);   // ERROR: signed

    // TRAP 1: bit_ceil(0) is 1, not 0 -- a surprise if you expected "no bits".
    std::cout << "bit_ceil(0) = " << std::bit_ceil(0u) << "  (not 0)\n";

    // TRAP 2: bit_ceil is UNDEFINED when the next power of two does not fit in
    // the type. For a 32-bit value the largest safe input is 2^31; above that
    // the result 2^32 does not fit and the call is UB. We do NOT run it:
    std::uint32_t too_big = (std::numeric_limits<std::uint32_t>::max() / 2) + 2; // > 2^31
    std::cout << "largest safe bit_ceil input is 2^31 = " << (1u << 31) << "\n";
    // std::cout << std::bit_ceil(too_big);   // <-- undefined behaviour (overflow)
    (void)too_big;

    // SAFE: keep inputs within range. 2^31 is itself a power of two, so bit_ceil
    // returns it unchanged.
    std::cout << "bit_ceil(2^31) = " << std::bit_ceil(1u << 31) << "\n";
}
