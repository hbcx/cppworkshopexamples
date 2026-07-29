#include <iostream>
#include <string>

int main() {
    // "café" written as explicit UTF-8 bytes so it does not depend on the source
    // file's encoding: 'c' 'a' 'f' then U+00E9 (é) = 0xC3 0xA9.
    std::string cafe = "caf\xC3\xA9";

    std::cout << "text: " << cafe << '\n';                  // valid UTF-8, renders as café
    std::cout << "size() in bytes:  " << cafe.size() << '\n';   // 5, not 4

    // The library will not count characters for you, but you can: a UTF-8 code
    // point starts on any byte that is NOT a 10xxxxxx continuation byte.
    int codepoints = 0;
    for (unsigned char c : cafe)
        if ((c & 0xC0) != 0x80) ++codepoints;
    std::cout << "code points:      " << codepoints << '\n';    // 4

    // The é is two bytes, so a byte-offset substr can slice through it.
    std::string cut = cafe.substr(0, 4);                    // keeps only the first byte of é
    unsigned char lastByte = static_cast<unsigned char>(cut.back());
    std::cout << "substr(0,4) ends mid-character? "
              << ((lastByte & 0x80) ? "yes" : "no") << '\n';     // yes -- a lone lead byte
    return 0;
}
