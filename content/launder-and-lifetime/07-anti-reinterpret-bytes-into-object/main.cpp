// Anti-pattern: reinterpret_cast a byte buffer to a struct pointer and read it.
//
// The cast does not create an object -- no Header ever began its lifetime at
// those bytes, so reading through the pointer is undefined even when the bytes
// look right and the address is aligned. memcpy into a real object (or C++23
// start_lifetime_as) is the defined way.

#include <cstdint>
#include <cstring>
#include <iostream>

struct Header {
    std::uint32_t magic;
    std::uint32_t size;
};

int main() {
    Header original{0xCAFEBABEu, 256};
    alignas(Header) unsigned char buffer[sizeof(Header)];
    std::memcpy(buffer, &original, sizeof original);

    // BAD (undefined -- described, not run):
    //   const Header* h = reinterpret_cast<const Header*>(buffer);
    //   use h->magic;   // no Header object lives here; reading it is undefined

    // GOOD: memcpy the bytes into a real Header, which starts its lifetime.
    Header h;
    std::memcpy(&h, buffer, sizeof h);

    std::cout << std::hex << "magic = 0x" << h.magic << '\n';
    std::cout << std::dec << "size  = " << h.size << '\n';
}
