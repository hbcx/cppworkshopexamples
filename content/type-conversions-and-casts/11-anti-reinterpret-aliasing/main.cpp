#include <iostream>
#include <cstdint>
#include <cstring>   // std::memcpy

int main() {
    float f = 1.0f;

    // BAD, described but NOT run: reinterpret the float's storage as a uint32_t
    // through a typed pointer. This breaks strict aliasing and is undefined
    // behavior -- the optimizer may assume the two never overlap.
    //   std::uint32_t bits = *reinterpret_cast<std::uint32_t*>(&f);   // UB

    // GOOD: copy the bytes into a uint32_t. Reading through bytes is allowed, so
    // this is defined and gives the IEEE-754 bit pattern.
    std::uint32_t bits;
    std::memcpy(&bits, &f, sizeof bits);

    std::cout << std::hex << "bits of 1.0f via memcpy = 0x" << bits << std::dec << "\n";  // 0x3f800000

    // In C++20, std::bit_cast<std::uint32_t>(f) does exactly this in one expression.
    return 0;
}
