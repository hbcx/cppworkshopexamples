// Anti-pattern: reinterpret_cast a byte buffer to a struct pointer.
//
// A record at some offset in a received buffer is rarely on the struct's
// boundary, and the raw bytes are not an object of that type anyway -- reading
// through the cast is misaligned and violates aliasing. memcpy into a real,
// aligned object instead.

#include <cstdint>
#include <cstring>   // std::memcpy
#include <iostream>

struct Header {
    std::uint32_t id;
    std::uint16_t len;
};

int main() {
    // A wire buffer holding a Header's bytes, deliberately at offset 1.
    Header original{0xDEADBEEFu, 42};
    unsigned char wire[1 + sizeof(Header)];
    std::memcpy(wire + 1, &original, sizeof original);

    // BAD (undefined -- described, not run):
    //   Header* h = reinterpret_cast<Header*>(wire + 1);  // misaligned pointer
    //   use h->id;   // faults on ARM, undefined everywhere

    // GOOD: copy the bytes out into an aligned Header.
    Header parsed;
    std::memcpy(&parsed, wire + 1, sizeof parsed);

    std::cout << std::hex << "parsed.id  = 0x" << parsed.id << std::dec << '\n';
    std::cout << "parsed.len = " << parsed.len << '\n';
    std::cout << "&parsed aligned for Header? "
              << (reinterpret_cast<std::uintptr_t>(&parsed) % alignof(Header) == 0
                      ? "yes" : "no")
              << '\n';
}
