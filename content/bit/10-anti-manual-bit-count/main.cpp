#include <bit>
#include <cstdint>
#include <iostream>

// TRAP: hand-rolled bit counting. It works, but it is slower (popcount is
// usually a single CPU instruction), and loops like this are easy to get wrong
// on width or signedness -- a signed value shifted right keeps its sign bit and
// the loop never ends.
static int manual_popcount(std::uint32_t v) {
    int count = 0;
    while (v) { count += static_cast<int>(v & 1u); v >>= 1; }
    return count;
}
static int manual_bit_width(std::uint32_t v) {
    int w = 0;
    while (v) { ++w; v >>= 1; }
    return w;
}

int main() {
    std::uint32_t v = 0b1011'0100u;

    // The loops agree with the standard functions -- so prefer the standard
    // ones: clearer, correct by construction, and typically one instruction.
    std::cout << "manual popcount = " << manual_popcount(v)
              << ", std::popcount = " << std::popcount(v) << "\n";
    std::cout << "manual bit_width = " << manual_bit_width(v)
              << ", std::bit_width = " << std::bit_width(v) << "\n";
}
