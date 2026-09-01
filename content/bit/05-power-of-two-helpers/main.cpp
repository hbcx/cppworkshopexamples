#include <bit>
#include <cstdint>
#include <iostream>

int main() {
    // has_single_bit: is the value a power of two (exactly one bit set)?
    for (std::uint32_t v : {1u, 2u, 3u, 8u, 15u, 16u})
        std::cout << v << (std::has_single_bit(v) ? " is" : " is not")
                  << " a power of two\n";

    // bit_width: bits needed to represent the value = floor(log2(v)) + 1.
    // bit_width(0) is 0.
    std::cout << "bit_width: 1->" << std::bit_width(1u)
              << " 16->" << std::bit_width(16u)
              << " 17->" << std::bit_width(17u) << "\n";           // 1, 5, 5

    // bit_ceil rounds UP to the next power of two; bit_floor rounds DOWN. The
    // classic tool for sizing a hash table or buffer to a power of two.
    std::cout << "bit_ceil(17)=" << std::bit_ceil(17u)
              << " bit_floor(17)=" << std::bit_floor(17u) << "\n"; // 32, 16
    std::cout << "bit_ceil(16)=" << std::bit_ceil(16u)
              << "  (already a power of two)\n";                   // 16

    // With a power-of-two capacity, index % cap becomes index & (cap - 1).
    std::uint32_t cap = std::bit_ceil(50u);   // 64
    std::cout << "capacity for 50 items = " << cap
              << ", index 130 maps to slot " << (130u & (cap - 1)) << "\n";  // 2
}
