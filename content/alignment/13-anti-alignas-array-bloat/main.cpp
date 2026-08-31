// Anti-pattern: over-aligning the element type of an array.
//
// sizeof must be a multiple of alignof so that array[i] stays aligned. So
// alignas(64) on a 4-byte struct pads each element to 64 bytes -- most of the
// array becomes padding. Over-align the single hot object, not array elements.

#include <iostream>

struct Small {
    int x;   // 4 bytes, 4-byte alignment
};

struct alignas(64) Padded {
    int x;   // same data, but each object is now padded to 64 bytes
};

int main() {
    std::cout << "sizeof(Small)  = " << sizeof(Small)  << '\n';
    std::cout << "sizeof(Padded) = " << sizeof(Padded) << "  (padded up to 64)\n";

    Small  a[10];
    Padded b[10];
    std::cout << "10 Small  = " << sizeof(a) << " bytes\n";
    std::cout << "10 Padded = " << sizeof(b) << " bytes for the same 10 ints\n";
    std::cout << "wasted: " << (sizeof(b) - sizeof(a)) << " bytes of padding\n";
}
