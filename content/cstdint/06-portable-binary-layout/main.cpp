#include <cstdint>
#include <iostream>
#include <array>

// A file header or a network packet has an EXACT byte layout that both ends
// must agree on. Building it from int/long would make the size depend on the
// compiler; fixed-width types pin every field. We also serialize field by field
// with explicit big-endian shifts, so the bytes come out the same regardless of
// the host's byte order. (Never memcpy a whole struct to the wire -- padding
// and the machine's endianness would leak into the format.)
struct PacketHeader {
    std::uint16_t version;
    std::uint16_t flags;
    std::uint32_t payloadLength;
};

std::array<unsigned char, 8> serialize(const PacketHeader& h) {
    std::array<unsigned char, 8> out{};
    out[0] = static_cast<unsigned char>(h.version >> 8);
    out[1] = static_cast<unsigned char>(h.version & 0xFF);
    out[2] = static_cast<unsigned char>(h.flags >> 8);
    out[3] = static_cast<unsigned char>(h.flags & 0xFF);
    out[4] = static_cast<unsigned char>(h.payloadLength >> 24);
    out[5] = static_cast<unsigned char>(h.payloadLength >> 16);
    out[6] = static_cast<unsigned char>(h.payloadLength >> 8);
    out[7] = static_cast<unsigned char>(h.payloadLength & 0xFF);
    return out;
}

PacketHeader deserialize(const std::array<unsigned char, 8>& in) {
    PacketHeader h{};
    h.version = static_cast<std::uint16_t>((in[0] << 8) | in[1]);
    h.flags   = static_cast<std::uint16_t>((in[2] << 8) | in[3]);
    h.payloadLength = (static_cast<std::uint32_t>(in[4]) << 24)
                    | (static_cast<std::uint32_t>(in[5]) << 16)
                    | (static_cast<std::uint32_t>(in[6]) << 8)
                    |  static_cast<std::uint32_t>(in[7]);
    return h;
}

int main() {
    PacketHeader h{1, 0x00FF, 4096};

    auto bytes = serialize(h);
    std::cout << "wire bytes (hex):";
    for (unsigned char b : bytes)
        std::cout << ' ' << std::hex << static_cast<int>(b);
    std::cout << std::dec << "\n";

    PacketHeader r = deserialize(bytes);
    std::cout << "version = " << r.version << "\n";
    std::cout << "flags   = " << r.flags   << "\n";
    std::cout << "length  = " << r.payloadLength << "\n";

    std::cout << "\nevery field has a fixed size, so the format is stable:\n";
    std::cout << "version(2) + flags(2) + length(4) = "
              << sizeof(h.version) + sizeof(h.flags) + sizeof(h.payloadLength)
              << " bytes on the wire\n";
}
