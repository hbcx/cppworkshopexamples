#include <iostream>
#include <cstdint>
#include <type_traits>

// A scoped enum makes a clean set of bit flags -- but because it does not convert
// to int, you must define the bitwise operators yourself. That is the price for
// type safety: you cannot accidentally OR a Permission with an unrelated number.

enum class Permission : std::uint8_t {
    None    = 0,
    Read    = 1 << 0,   // 1
    Write   = 1 << 1,   // 2
    Execute = 1 << 2,   // 4
};

// Define the operators once. The pattern: cast to the underlying type, do the
// bit work, cast back to the enum.
constexpr Permission operator|(Permission a, Permission b) {
    using U = std::underlying_type<Permission>::type;
    return static_cast<Permission>(static_cast<U>(a) | static_cast<U>(b));
}
constexpr Permission operator&(Permission a, Permission b) {
    using U = std::underlying_type<Permission>::type;
    return static_cast<Permission>(static_cast<U>(a) & static_cast<U>(b));
}
constexpr bool any(Permission p) { return p != Permission::None; }

int main() {
    Permission p = Permission::Read | Permission::Write;   // combine flags

    std::cout << "has Read?    " << any(p & Permission::Read)    << "\n"; // 1
    std::cout << "has Write?   " << any(p & Permission::Write)   << "\n"; // 1
    std::cout << "has Execute? " << any(p & Permission::Execute) << "\n"; // 0

    std::cout << "raw bits = " << static_cast<int>(p) << "\n";           // 3
    return 0;
}
