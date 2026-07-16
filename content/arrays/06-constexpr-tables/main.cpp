#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>

// A constexpr function that builds a whole table. It needs a mutable local and
// a return -- an array cannot be filled by an initializer list computed in a
// loop -- which is why std::array's default constructibility matters here.
constexpr std::array<std::uint32_t, 16> make_squares() {
    std::array<std::uint32_t, 16> table{};      // {} matters: value-initialized
    for (std::uint32_t i = 0; i < table.size(); ++i) {
        table[i] = i * i;                       // operator[] is constexpr since C++17
    }
    return table;
}

// The loop above runs in the COMPILER. What lands in the binary is the finished
// data, in read-only memory: no static initializer, no first-call guard.
constexpr auto kSquares = make_squares();

// Free proof that it really happened at compile time -- if any of this were
// deferred to runtime, the build would fail here rather than produce a slower
// program.
static_assert(kSquares[0] == 0, "table must be computed at compile time");
static_assert(kSquares[12] == 144, "table must be computed at compile time");
static_assert(kSquares.size() == 16, "size is part of the type");

// The same trick for a table whose formula nobody wants to work out by hand:
// the parity (population count mod 2) of every byte value.
constexpr std::array<bool, 256> make_parity() {
    std::array<bool, 256> table{};
    for (std::size_t value = 0; value < table.size(); ++value) {
        bool odd = false;
        for (std::size_t bit = 0; bit < 8; ++bit) {
            if ((value >> bit) & 1u) {
                odd = !odd;
            }
        }
        table[value] = odd;
    }
    return table;
}

constexpr auto kParity = make_parity();
static_assert(!kParity[0], "0 has no bits set");
static_assert(kParity[1], "1 has one bit set");
static_assert(!kParity[3], "3 has two bits set");

int main() {
    std::cout << "squares: ";
    for (std::uint32_t s : kSquares) {
        std::cout << s << ' ';
    }
    std::cout << '\n';

    // At runtime this is one load from read-only memory -- the multiply never
    // happens, because it already happened during the build.
    std::cout << "kSquares[7]  = " << kSquares[7] << '\n';
    std::cout << "kParity[7]   = " << kParity[7] << "  (7 = 0b111, three bits, odd)\n";
    std::cout << "kParity[255] = " << kParity[255] << "  (eight bits, even)\n";
    return 0;
}
