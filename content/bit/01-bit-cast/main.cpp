#include <bit>
#include <cstdint>
#include <iostream>

int main() {
    // std::bit_cast reinterprets the bits of one object as another type of the
    // SAME size, with no undefined behaviour -- unlike reinterpret_cast or a
    // union, which break strict aliasing. It needs both types trivially
    // copyable and sizeof(To) == sizeof(From), and it is constexpr.
    float f = 1.5f;
    auto bits = std::bit_cast<std::uint32_t>(f);
    std::cout << "1.5f as raw bits = 0x" << std::hex << bits << std::dec << "\n";

    // The IEEE-754 single-precision layout: sign | 8-bit exponent | 23-bit
    // mantissa. We can pull the fields straight out of the bit pattern.
    std::uint32_t sign     = bits >> 31;
    std::uint32_t exponent = (bits >> 23) & 0xFFu;
    std::uint32_t mantissa = bits & 0x7FFFFFu;
    std::cout << "sign=" << sign << " exponent=" << exponent
              << " mantissa=0x" << std::hex << mantissa << std::dec << "\n";

    // And back again, bit-for-bit.
    float roundtrip = std::bit_cast<float>(bits);
    std::cout << "back to float = " << roundtrip << "\n";
}
