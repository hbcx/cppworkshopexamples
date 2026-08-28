#include <iostream>
#include <bit>
#include <cstdint>

int main() {
    // Read the IEEE-754 bit pattern of a float as a 32-bit integer -- one call, no
    // pointers, no aliasing violation.
    float f = 1.0f;
    std::uint32_t bits = std::bit_cast<std::uint32_t>(f);

    std::cout << std::hex;
    std::cout << "bits of 1.0f = 0x" << bits << "\n";     // 0x3f800000
    std::cout << std::dec;

    // And back again: the round trip returns the exact same value.
    float again = std::bit_cast<float>(bits);
    std::cout << "round-trips back to 1.0f? " << (again == f) << "\n";   // 1

    // Both types are trivially copyable and the same size, which is the whole
    // requirement -- bit_cast checks it at compile time.
    static_assert(sizeof(float) == sizeof(std::uint32_t));
    return 0;
}
