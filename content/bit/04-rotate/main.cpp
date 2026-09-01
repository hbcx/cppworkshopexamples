#include <bit>
#include <cstdint>
#include <iostream>
#include <bitset>

int main() {
    std::uint8_t x = 0b1000'0001u;   // 8-bit value: bits 0 and 7 set

    // rotl rotates left: bits leaving the top re-enter at the bottom. No bits
    // are lost, unlike a plain shift. std::rotl keeps the operand's real width
    // (here 8 bits), so the rotation wraps within the byte.
    std::cout << "x       = " << std::bitset<8>(x) << "\n";
    std::cout << "rotl 1  = " << std::bitset<8>(std::rotl(x, 1)) << "\n";   // 00000011
    std::cout << "rotr 1  = " << std::bitset<8>(std::rotr(x, 1)) << "\n";   // 11000000

    // The count wraps modulo the width, and 0 or a full-width count is well
    // defined -- it returns the value unchanged. (The hand-rolled rotate is UB
    // here; see the trap.)
    std::cout << "rotl 0  = " << std::bitset<8>(std::rotl(x, 0)) << "  (unchanged)\n";
    std::cout << "rotl 8  = " << std::bitset<8>(std::rotl(x, 8)) << "  (full turn)\n";
    std::cout << "rotl 9  = " << std::bitset<8>(std::rotl(x, 9)) << "  (same as rotl 1)\n";
}
