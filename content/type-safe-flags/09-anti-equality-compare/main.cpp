#include <iostream>
#include <type_traits>

enum class Perm : unsigned { Read = 1u << 0, Write = 1u << 1, Execute = 1u << 2 };
using U = std::underlying_type_t<Perm>;
constexpr Perm operator|(Perm a, Perm b) {
    return static_cast<Perm>(static_cast<U>(a) | static_cast<U>(b));
}
constexpr Perm operator&(Perm a, Perm b) {
    return static_cast<Perm>(static_cast<U>(a) & static_cast<U>(b));
}
constexpr bool has(Perm set, Perm flag) { return (set & flag) == flag; }

// A guard that should allow anyone who has the Read bit.
bool canRead_wrong(Perm p) { return p == Perm::Read; }    // exact match of the whole value
bool canRead_right(Perm p) { return has(p, Perm::Read); }  // tests just the Read bit

int main() {
    Perm p = Perm::Read | Perm::Write;   // Read is set, plus Write

    // --- the trap: == is an exact match of the WHOLE value ---
    // p is Read|Write (0b011), not Read (0b001), so == reports "no Read" even
    // though the Read bit is plainly set.
    std::cout << "canRead_wrong (==):  " << canRead_wrong(p) << '\n';   // 0  (wrong)

    // --- the fix: test the bit with has() / operator& ---
    std::cout << "canRead_right (has): " << canRead_right(p) << '\n';   // 1

    // == is only right when you mean exactly these flags and nothing else.
    std::cout << "is EXACTLY Read only? " << (p == Perm::Read) << '\n'; // 0
    return 0;
}
