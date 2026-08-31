// memcpy into a trivially copyable object begins its lifetime and fills it in
// one defined step -- the portable way to read a struct out of a byte buffer.
// (C++23 std::start_lifetime_as does this in place, with no copy.)

#include <cstdint>
#include <cstring>   // std::memcpy
#include <iostream>
#include <type_traits>

struct Packet {
    std::uint16_t type;
    std::uint16_t len;
    std::uint32_t seq;
};

int main() {
    static_assert(std::is_trivially_copyable<Packet>::value, "must be trivially copyable");

    // A byte buffer holding a Packet's bytes (built from a real Packet so the
    // layout matches, no endianness assumptions).
    Packet original{0x1234, 40, 0xABCDEF01u};
    unsigned char wire[sizeof(Packet)];
    std::memcpy(wire, &original, sizeof original);

    // Read the bytes back into a Packet. This starts pkt's lifetime and copies.
    Packet pkt;
    std::memcpy(&pkt, wire, sizeof pkt);

    std::cout << std::hex;
    std::cout << "type = 0x" << pkt.type << '\n';
    std::cout << "len  = " << std::dec << pkt.len << '\n';
    std::cout << std::hex << "seq  = 0x" << pkt.seq << '\n';
}
