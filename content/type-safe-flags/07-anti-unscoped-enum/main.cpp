#include <iostream>

// Unscoped enums: the enumerators leak into the enclosing scope AND each one
// implicitly converts to int. That makes bitwise ops "just work" -- but the
// result is a bare int, and two unrelated flag sets mix with no error.
enum FileFlag { FRead = 1u << 0, FWrite = 1u << 1, FCreate = 1u << 2 };
enum NetFlag  { NFast = 1u << 0, NSecure = 1u << 1 };

int main() {
    // FRead | FWrite is an int, not a FileFlag: the type is gone the moment you
    // combine, so a function taking FileFlag cannot be protected by the type.
    auto combined = FRead | FWrite;
    std::cout << "type is int now, combined = " << combined << '\n';   // 3

    // --- the trap: two DIFFERENT flag vocabularies mix into one int ---
    // Both enums decay to int, so this compiles with no error even though the
    // two constants come from unrelated flag sets and the sum is meaningless.
    int nonsense = FCreate | NSecure;     // FileFlag OR-ed with NetFlag
    std::cout << "mixing FileFlag and NetFlag compiles, nonsense = " << nonsense << '\n';  // 6

    // --- the fix: enum class keeps the type ---
    // enum class Permission : unsigned { Read = 1u<<0, Write = 1u<<1, Create = 1u<<2 };
    // Permission p = Permission::Read | Permission::Write;   // stays Permission
    // Permission::Read | NetOption::Fast;   // compile error: two different enums
    std::cout << "with enum class, mixing two flag enums is a compile error\n";
    return 0;
}
