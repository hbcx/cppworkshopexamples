// Anti-pattern: writing one union member and reading another.
//
// In C++ only the last-written union member is alive; the others are just
// storage, so reading them is undefined (it is a C habit, not valid C++). To
// reinterpret a value's bytes, memcpy into the target type (or std::bit_cast in
// C++20).

#include <cstdint>
#include <cstring>
#include <iostream>

union FloatBits {
    float f;
    std::uint32_t bits;
};

int main() {
    // BAD (undefined -- described, not run):
    //   FloatBits u;
    //   u.f = 1.5f;                 // f is the active member
    //   std::uint32_t b = u.bits;   // reading the inactive member: undefined

    // GOOD: memcpy the float's bytes into an integer.
    float value = 1.5f;
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof bits);

    std::cout << std::hex << "bit pattern of 1.5f = 0x" << bits << '\n';
    // (std::bit_cast<std::uint32_t>(value) is the C++20 one-liner for the same.)
}
