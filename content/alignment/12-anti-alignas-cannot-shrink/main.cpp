// Anti-pattern: reaching for alignas to pack a struct.
//
// alignas can only RAISE alignment. alignas(1) on an int is ill-formed -- below
// the natural 4 -- so it is no way to remove padding. Packing is a separate
// tool: a compiler pragma. Both g++ and clang accept #pragma pack.

#include <iostream>

// alignas raising alignment: this works.
struct Raised {
    alignas(8) char c;   // 1 -> 8 is allowed
};

// BAD (ill-formed -- described, not compiled):
//   struct Shrunk { alignas(1) int i; };   // 1 < alignof(int): rejected

// GOOD: pack with a pragma to drop the padding.
#pragma pack(push, 1)
struct Packed {
    char c;
    int  i;
};
#pragma pack(pop)

struct Normal {
    char c;
    int  i;
};

int main() {
    std::cout << "alignof(Raised) = " << alignof(Raised) << "  (alignas raised it)\n";
    std::cout << "sizeof(Normal)  = " << sizeof(Normal)  << "  (padded)\n";
    std::cout << "sizeof(Packed)  = " << sizeof(Packed)  << "  (pragma pack, no padding)\n";
}
