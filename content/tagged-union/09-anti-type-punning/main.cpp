#include <cstdint>
#include <cstring>
#include <iostream>

int main() {
    // A common C trick: store an integer in one union member, then read a float
    // from another to reinterpret the bit pattern.
    union Pun {
        std::uint32_t u;
        float         f;
    };

    Pun p;
    p.u = 0x40490FDBu;   // the bit pattern of 3.14159f

    // --- the trap (NOT read here: reading the inactive member is UB in C++) ---
    // In C, reading p.f after writing p.u is a defined reinterpretation. In C++
    // only the active member (p.u) may be read, so this is undefined behaviour,
    // even though it happens to "work" on common compilers:
    //   float wrong = p.f;

    // --- the fix: copy the bytes explicitly with std::memcpy ---
    float value;
    std::memcpy(&value, &p.u, sizeof value);   // reads the ACTIVE member's bytes
    std::cout << "reinterpreted bits as float: " << value << '\n';   // 3.14159

    // std::bit_cast (C++20) is the same reinterpretation in one expression:
    //   float value = std::bit_cast<float>(p.u);
    std::cout << "use memcpy or std::bit_cast to pun bits in C++, not a union\n";
    return 0;
}
