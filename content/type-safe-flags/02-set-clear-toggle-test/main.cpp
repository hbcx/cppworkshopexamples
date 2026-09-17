#include <iostream>
#include <type_traits>

enum class Access : unsigned {
    None   = 0,
    Read   = 1u << 0,
    Write  = 1u << 1,
    Append = 1u << 2,
    Lock   = 1u << 3,
};

using U = std::underlying_type_t<Access>;

// The four base operators...
constexpr Access operator|(Access a, Access b) { return static_cast<Access>(static_cast<U>(a) | static_cast<U>(b)); }
constexpr Access operator&(Access a, Access b) { return static_cast<Access>(static_cast<U>(a) & static_cast<U>(b)); }
constexpr Access operator^(Access a, Access b) { return static_cast<Access>(static_cast<U>(a) ^ static_cast<U>(b)); }
constexpr Access operator~(Access a)           { return static_cast<Access>(~static_cast<U>(a)); }

// ...and the compound forms the call sites actually use.
constexpr Access& operator|=(Access& a, Access b) { return a = a | b; }
constexpr Access& operator&=(Access& a, Access b) { return a = a & b; }
constexpr Access& operator^=(Access& a, Access b) { return a = a ^ b; }

constexpr bool has(Access set, Access flag) { return (set & flag) == flag; }
constexpr bool any(Access set)              { return set != Access::None; }

int main() {
    Access a = Access::None;

    a |= Access::Read;                    // set
    a |= Access::Write;                   // set
    std::cout << "read+write, has Write: " << has(a, Access::Write) << '\n';   // 1

    a &= ~Access::Write;                  // clear Write
    std::cout << "after clear, has Write: " << has(a, Access::Write) << '\n';  // 0

    a ^= Access::Lock;                    // toggle Lock on
    std::cout << "toggled Lock on:  " << has(a, Access::Lock) << '\n';         // 1
    a ^= Access::Lock;                    // toggle Lock off
    std::cout << "toggled Lock off: " << has(a, Access::Lock) << '\n';         // 0

    // any / all over a multi-bit mask.
    Access rw = Access::Read | Access::Write;
    std::cout << "has ALL of read+write: " << ((a & rw) == rw) << '\n';        // 0 (write cleared)
    std::cout << "has ANY of read+write: " << any(a & rw) << '\n';             // 1 (read still set)

    std::cout << "final raw bits: " << static_cast<U>(a) << '\n';              // 1 (Read)
    return 0;
}
