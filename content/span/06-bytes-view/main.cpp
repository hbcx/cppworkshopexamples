#include <array>
#include <cstdint>
#include <iostream>
#include <span>

int main() {
    // as_bytes views the SAME memory as read-only bytes -- no copy. Useful for
    // serialization, hashing, or writing a buffer to a socket or file.
    std::array<std::uint16_t, 3> data{0x0102, 0x0304, 0x0506};
    std::span<const std::byte> bytes = std::as_bytes(std::span(data));

    std::cout << "elements: " << data.size() << ", bytes: " << bytes.size() << '\n';

    // Sum the raw bytes. The byte ORDER depends on endianness, but the sum does
    // not, so this output is stable across platforms and compilers.
    int checksum = 0;
    for (std::byte b : bytes) checksum += std::to_integer<int>(b);
    std::cout << "byte checksum: " << checksum << '\n'; // 1+2+3+4+5+6 = 21

    // as_writable_bytes gives a MUTABLE byte view: zero the buffer in place.
    std::array<std::uint16_t, 3> buf{0xFFFF, 0xFFFF, 0xFFFF};
    std::span<std::byte> writable = std::as_writable_bytes(std::span(buf));
    for (std::byte& b : writable) b = std::byte{0};
    std::cout << "after zero: " << buf[0] << ' ' << buf[1] << ' ' << buf[2] << '\n';
    return 0;
}
