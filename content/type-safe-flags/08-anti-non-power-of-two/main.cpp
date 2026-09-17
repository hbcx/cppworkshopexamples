#include <iostream>
#include <type_traits>

// Flag values MUST be single, distinct bits. Here Execute was given 3 by
// counting up (1, 2, 3) instead of using separate bits (1, 2, 4). 3 is 0b011 --
// the SAME bits as Read(1) | Write(2) -- so Execute cannot be told apart from
// "Read and Write together".
enum class Perm : unsigned {
    Read    = 1,   // 0b001
    Write   = 2,   // 0b010
    Execute = 3,   // 0b011  <-- BUG: overlaps Read and Write; should be 4 (0b100)
};
using U = std::underlying_type_t<Perm>;
constexpr Perm operator|(Perm a, Perm b) {
    return static_cast<Perm>(static_cast<U>(a) | static_cast<U>(b));
}
constexpr bool has(Perm set, Perm flag) {
    return (static_cast<U>(set) & static_cast<U>(flag)) == static_cast<U>(flag);
}

int main() {
    Perm rw = Perm::Read | Perm::Write;    // 0b011

    // --- the symptom (fully defined, just wrong) ---
    // has() asks "are all of Execute's bits set?". Execute is 0b011 and rw is
    // also 0b011, so this is TRUE even though Execute was never granted.
    std::cout << "read+write, has Execute (buggy): " << has(rw, Perm::Execute) << '\n';   // 1

    // --- the fix: one bit per flag, written as 1u << n ---
    // enum class Perm : unsigned { Read = 1u<<0, Write = 1u<<1, Execute = 1u<<2 };
    // then Execute is 0b100, shares no bits with Read|Write, and has() is right.
    std::cout << "with 1u<<n values, has Execute would be 0\n";
    return 0;
}
