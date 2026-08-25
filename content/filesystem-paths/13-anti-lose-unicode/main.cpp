#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main() {
    // Anti-pattern: round-tripping a non-ASCII path through the plain narrow
    // string(). On Windows string() re-encodes to the active code page, which
    // may not be able to represent the characters -- so 'l-stroke', 'u-umlaut'
    // or CJK names come back mangled or throw. The bytes are safe only through
    // the UTF-8 API.

    // Construct from UTF-8 bytes with u8path (C++17): 'l-with-stroke' is the two
    // bytes 0xC5 0x82.
    fs::path name = fs::u8path("raport-\xC5\x82-2026.txt");

    // u8string() gives UTF-8 on every platform -- a stable, lossless spelling.
    std::string utf8 = name.u8string();
    std::cout << "utf8 byte length: " << utf8.size() << '\n';   // 18

    // Rebuilding a path from that UTF-8 gives back an equal path -- the round
    // trip is lossless when you stay in UTF-8 the whole way.
    fs::path again = fs::u8path(utf8);
    std::cout << "round trip equal: " << (again == name) << '\n';   // 1
    return 0;
}
