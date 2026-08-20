#include <format>
#include <iostream>

int main() {
    int n = 255;
    // Integer bases: d dec, x/X hex, o octal, b binary.
    std::cout << std::format("dec {0:d}  hex {0:x}  HEX {0:X}  oct {0:o}  bin {0:b}\n", n);

    // The # flag adds the base prefix (0x, 0, 0b).
    std::cout << std::format("with prefix: {0:#x} {0:#o} {0:#b}\n", n);   // 0xff 0377 0b11111111

    // A sign and zero-padding to a width.
    std::cout << std::format("sign + zero-pad: {:+06d}\n", 42);          // +00042

    double d = 12345.6789;
    // Float types: f fixed, e scientific, g general.
    std::cout << std::format("fixed {:.2f}  sci {:.2e}  general {:g}\n", d, d, d);

    // bool prints as text; c formats an integer as its character.
    std::cout << std::format("bool: {}  char from 65: {:c}\n", true, 65); // true, A
    return 0;
}
