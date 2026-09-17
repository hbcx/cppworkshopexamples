#include <bitset>
#include <iostream>
#include <type_traits>

// The enum-class bitmask: small, named, type-safe.
enum class Opt : unsigned { A = 1u << 0, B = 1u << 1, C = 1u << 2 };
using U = std::underlying_type_t<Opt>;
constexpr Opt operator|(Opt a, Opt b) {
    return static_cast<Opt>(static_cast<U>(a) | static_cast<U>(b));
}
constexpr bool has(Opt s, Opt f) {
    return (static_cast<U>(s) & static_cast<U>(f)) == static_cast<U>(f);
}

int main() {
    // 1) enum-class bitmask: a handful of NAMED options, checked by name, and
    //    the compiler stops you mixing this enum with any other flag enum.
    Opt o = Opt::A | Opt::C;
    std::cout << "enum mask has C: " << has(o, Opt::C) << '\n';               // 1

    // 2) std::bitset<N>: a set indexed by NUMBER, good when there are many bits
    //    or the index is computed. No names, and no type safety between sets --
    //    a bitset of permissions and a bitset of features are the same type.
    std::bitset<128> features;
    features.set(3);
    features.set(64);
    features.set(100);
    std::cout << "bitset count: " << features.count()
              << ", bit 64 set: " << features.test(64) << '\n';              // 3, 1

    // 3) raw unsigned mask: what a C API hands you. Fast and ABI-stable, but the
    //    type is just unsigned, so nothing stops a wrong constant being OR-ed in.
    unsigned raw = (1u << 0) | (1u << 2);
    std::cout << "raw mask bit 2 set: " << ((raw & (1u << 2)) != 0) << '\n';  // 1

    // Rule of thumb: a small fixed set of named options -> enum-class bitmask;
    // many bits addressed by index -> std::bitset; a C ABI boundary -> raw mask.
    return 0;
}
