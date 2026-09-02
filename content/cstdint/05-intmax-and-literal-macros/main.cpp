#include <cstdint>
#include <iostream>

// intmax_t / uintmax_t are the widest integer types the implementation has --
// a safe "hold any integer value" target. The INTn_C / UINTn_C macros build a
// literal of the matching least-width type, so a big constant has enough bits
// before it is used. The INTn_MAX / UINTn_MAX macros are the fixed-width limits
// as constant expressions (they even work in #if).
int main() {
    std::intmax_t  big  = INTMAX_C(9000000000);    // > 2^31, needs more than 32 bits
    std::uintmax_t ubig = UINTMAX_C(18000000000);

    std::cout << "intmax_t  value  = " << big  << "\n";
    std::cout << "uintmax_t value  = " << ubig << "\n";
    std::cout << "sizeof(intmax_t) = " << sizeof(std::intmax_t) << " bytes\n";

    std::cout << "\nfixed-width limit macros:\n";
    std::cout << "INT32_MAX  = " << INT32_MAX  << "\n";
    std::cout << "UINT32_MAX = " << UINT32_MAX << "\n";
    std::cout << "INT64_MAX  = " << INT64_MAX  << "\n";

    // A bare 1 is an int; shifting it by 40 would overflow. UINT64_C makes the
    // literal 64-bit first, so the shift is well defined.
    std::uint64_t oneAt40 = UINT64_C(1) << 40;
    std::cout << "\nUINT64_C(1) << 40 = " << oneAt40 << "\n";
}
