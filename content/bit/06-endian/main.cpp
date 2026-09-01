#include <bit>
#include <iostream>

int main() {
    // std::endian reports the byte order of scalar types, known at COMPILE time,
    // so you can branch with if constexpr and pay nothing at run time.
    if constexpr (std::endian::native == std::endian::little)
        std::cout << "this platform is little-endian\n";
    else if constexpr (std::endian::native == std::endian::big)
        std::cout << "this platform is big-endian\n";
    else
        std::cout << "this platform is mixed-endian\n";

    std::cout << "little == big? " << (std::endian::little == std::endian::big) << "\n"; // 0

    // Typical use: network byte order is big-endian, so wire data needs a swap
    // unless this machine is already big-endian. Decided entirely at compile time.
    constexpr bool needs_swap = std::endian::native != std::endian::big;
    std::cout << "big-endian wire data needs a swap here: " << needs_swap << "\n";
}
