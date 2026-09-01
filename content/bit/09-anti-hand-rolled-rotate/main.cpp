#include <bit>
#include <cstdint>
#include <iostream>
#include <bitset>

int main() {
    std::uint32_t x = 0x1u;

    // TRAP: the textbook hand-rolled rotate is
    //     (x << n) | (x >> (32 - n))
    // At n == 0 the second term shifts by 32 -- a shift by (or past) the type's
    // width is UNDEFINED BEHAVIOUR, and may return 0, x, or anything. It also
    // hard-codes the width 32, so it is silently wrong on any other type. We do
    // NOT run the n == 0 case:
    //     std::uint32_t bad = (x << 0) | (x >> (32 - 0));   // x >> 32 is UB

    // FIX: std::rotl / std::rotr are defined for every count -- including 0 and
    // counts at or beyond the width -- and use the operand's real width.
    std::cout << "rotl(x, 0)  = " << std::bitset<32>(std::rotl(x, 0))
              << "  (defined: unchanged)\n";
    std::cout << "rotl(x, 1)  = " << std::bitset<32>(std::rotl(x, 1)) << "\n";
    std::cout << "rotl(x, 32) = " << std::bitset<32>(std::rotl(x, 32))
              << "  (defined: full turn)\n";
    std::cout << "rotr(x, 1)  = " << std::bitset<32>(std::rotr(x, 1))
              << "  (bit wraps to the top)\n";
}
