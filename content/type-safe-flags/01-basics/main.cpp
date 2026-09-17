#include <iostream>
#include <type_traits>

// A scoped enum (enum class) whose enumerators are single bits. Scoped is the
// point: it does NOT implicitly convert to int, so `Read | Write` does not even
// compile until WE define the operators for this type -- and two different flag
// enums can never be mixed by accident.
enum class Permission : unsigned {
    None    = 0,
    Read    = 1u << 0,   // 0b001
    Write   = 1u << 1,   // 0b010
    Execute = 1u << 2,   // 0b100
};

// The bitwise operators, written by hand for this one enum. Each converts to the
// underlying integer, does the bit work, and converts back to Permission, so the
// result stays a Permission and never leaks out as a bare int.
constexpr Permission operator|(Permission a, Permission b) {
    using U = std::underlying_type<Permission>::type;
    return static_cast<Permission>(static_cast<U>(a) | static_cast<U>(b));
}
constexpr Permission operator&(Permission a, Permission b) {
    using U = std::underlying_type<Permission>::type;
    return static_cast<Permission>(static_cast<U>(a) & static_cast<U>(b));
}
constexpr Permission operator~(Permission a) {
    using U = std::underlying_type<Permission>::type;
    return static_cast<Permission>(~static_cast<U>(a));
}

// Test that every bit in `flag` is set in `set`.
constexpr bool has(Permission set, Permission flag) {
    return (set & flag) == flag;
}

int main() {
    Permission p = Permission::Read | Permission::Write;

    std::cout << "has Read:    " << has(p, Permission::Read) << '\n';     // 1
    std::cout << "has Write:   " << has(p, Permission::Write) << '\n';    // 1
    std::cout << "has Execute: " << has(p, Permission::Execute) << '\n';  // 0

    // Clear Write by AND-ing with its complement.
    p = p & ~Permission::Write;
    std::cout << "after clearing Write, has Write: " << has(p, Permission::Write) << '\n';  // 0

    // The safety: with no implicit int conversion, a stray int cannot slip in as
    // a Permission, and a different flag enum cannot be OR-ed in. Only the
    // operators we defined FOR Permission make `Permission | Permission` legal.
    std::cout << "raw bits now: " << static_cast<unsigned>(p) << '\n';    // 1
    return 0;
}
