#include <bit>
#include <cstdint>
#include <iostream>

int main() {
    // TRAP: assuming a 32-bit width. countl_zero, bit_width and friends work on
    // the ACTUAL width of the argument's type, so the same value in a
    // different-width type gives different results.
    std::uint8_t  small = 1;   // 8-bit
    std::uint32_t big   = 1;   // 32-bit

    std::cout << "countl_zero(uint8_t 1)  = " << std::countl_zero(small)
              << "  (width 8)\n";                                     // 7
    std::cout << "countl_zero(uint32_t 1) = " << std::countl_zero(big)
              << "  (width 32)\n";                                    // 31

    // So "31 - countl_zero(x)" as a highest-bit index is ONLY right for 32-bit.
    // The width-independent answer is bit_width, with no hard-coded number:
    std::cout << "bit_width(small) = " << std::bit_width(small)
              << ", bit_width(big) = " << std::bit_width(big) << "\n"; // 1, 1

    // Integer promotion is a related trap: a uint8_t in a raw shift promotes to
    // int and loses its 8-bit width; the <bit> functions keep the operand type.
    std::cout << "rotl(uint8_t 0x81, 1) = "
              << static_cast<int>(std::rotl(std::uint8_t{0x81}, 1)) << "\n";  // 3
}
