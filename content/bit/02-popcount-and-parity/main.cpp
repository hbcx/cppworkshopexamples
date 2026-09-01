#include <bit>
#include <cstdint>
#include <iostream>

int main() {
    // std::popcount counts the set (1) bits in an unsigned integer.
    std::uint32_t flags = 0b1011'0100u;
    std::cout << "popcount(0b10110100) = " << std::popcount(flags) << "\n";   // 4

    // Parity: whether the number of set bits is odd -- one popcount and a mask.
    std::cout << "odd number of 1s? " << (std::popcount(flags) & 1) << "\n";  // 0

    // Hamming distance: how many bit positions differ = popcount of the XOR.
    std::uint32_t a = 0b1100u, b = 0b1010u;
    std::cout << "Hamming distance(1100, 1010) = " << std::popcount(a ^ b) << "\n"; // 2

    // The edges: none set, and all set in a 32-bit value.
    std::cout << "popcount(0) = " << std::popcount(0u)
              << ", popcount(0xFFFFFFFF) = " << std::popcount(~std::uint32_t{0})
              << "\n";                                                        // 0, 32
}
