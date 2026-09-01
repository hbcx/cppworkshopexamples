#include <bit>
#include <cstdint>
#include <iostream>

int main() {
    std::uint32_t x = 0b0000'1000u;   // only bit 3 set

    // countr_zero: trailing zeros, counted from the least-significant end. For a
    // value with a single bit set, this is the index of that bit.
    std::cout << "countr_zero(0b1000) = " << std::countr_zero(x) << "\n";   // 3

    // countl_zero: leading zeros, from the most-significant end. For a 32-bit
    // type, 31 - countl_zero(x) is the position of the highest set bit.
    std::cout << "countl_zero(0b1000) = " << std::countl_zero(x) << "\n";   // 28
    std::cout << "highest set bit index = " << (31 - std::countl_zero(x)) << "\n"; // 3

    // countr_one / countl_one count runs of set bits instead of zeros.
    std::uint32_t y = 0b0011'1111u;   // six low bits set
    std::cout << "countr_one(0b111111) = " << std::countr_one(y) << "\n";   // 6

    // countl_zero(0) is the full width -- every bit is zero.
    std::cout << "countl_zero(0) = " << std::countl_zero(0u) << "\n";       // 32
}
