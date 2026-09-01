#include <bit>
#include <cstdint>
#include <iostream>

int main() {
    // std::byteswap (C++23) reverses the bytes of an integer -- the operation
    // you need to convert between little- and big-endian representations.
    std::uint16_t a = 0x00FFu;
    std::cout << std::hex << "byteswap(0x00FF)     = 0x" << std::byteswap(a) << "\n";   // FF00

    std::uint32_t b = 0x12345678u;
    std::cout << "byteswap(0x12345678) = 0x" << std::byteswap(b) << "\n";               // 78563412

    // Combined with std::endian: swap only when the host order differs from the
    // wire order. Here we produce the big-endian ("network order") bytes of a
    // value, correctly on either kind of machine.
    std::uint32_t host = 0x0A0B0C0Du;
    std::uint32_t wire = (std::endian::native == std::endian::big)
                             ? host : std::byteswap(host);
    std::cout << "0x0A0B0C0D as big-endian = 0x" << wire << std::dec << "\n";
}
