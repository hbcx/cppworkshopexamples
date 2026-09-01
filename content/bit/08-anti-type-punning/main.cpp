#include <bit>
#include <cstdint>
#include <cstring>
#include <iostream>

int main() {
    float f = 3.14f;

    // TRAP: reading a float's bits through an unrelated pointer type breaks the
    // strict-aliasing rule -- undefined behaviour, even though it often "works"
    // until the optimizer assumes the pointers cannot alias. The union trick
    // (write one member, read another) is also UB in C++, though allowed in C.
    // We do NOT run these:
    //     std::uint32_t bad = *reinterpret_cast<std::uint32_t*>(&f);        // UB
    //     union { float f; std::uint32_t u; } p{f};  auto bad = p.u;        // UB in C++

    // FIX: std::bit_cast is the defined way to reinterpret bits (C++20). Before
    // C++20, the portable defined route was std::memcpy into the target type.
    std::uint32_t good = std::bit_cast<std::uint32_t>(f);
    std::uint32_t viacopy = 0;
    std::memcpy(&viacopy, &f, sizeof viacopy);

    std::cout << "bit_cast bits = 0x" << std::hex << good << "\n";
    std::cout << "memcpy   bits = 0x" << viacopy << std::dec
              << "  (same value, both defined)\n";
}
