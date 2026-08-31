// Anti-pattern: capturing operator[] with auto.
//
// A bit has no address, so operator[] returns a proxy (std::bitset::reference)
// that reads and writes through the bitset. auto deduces that proxy, so the
// variable tracks the live bit instead of being an independent bool. Say bool.

#include <bitset>
#include <iostream>

int main() {
    std::bitset<8> b;
    b.set(0);                    // bit 0 = 1

    auto proxy = b[0];           // a proxy that ALIASES bit 0, not a bool copy
    bool snapshot = b[0];        // a real bool: the value of bit 0 right now

    b.reset(0);                  // bit 0 -> 0

    std::cout << std::boolalpha;
    std::cout << "proxy after reset:    " << proxy << "  (followed the bit)\n";
    std::cout << "bool copy after reset: " << snapshot << "  (a stable snapshot)\n";
}
