// Anti-pattern: indexing a bitset out of range with operator[].
//
// operator[] does no bounds check -- an index past the last bit is undefined.
// test() does the same read but validates the index and throws out_of_range.

#include <bitset>
#include <iostream>
#include <stdexcept>

int main() {
    std::bitset<8> b;
    b.set(3);

    // BAD (undefined -- described, not run):
    //   bool x = b[8];   // valid indices are 0..7; b[8] is out of range

    // GOOD: test() checks the index and throws when it is too big.
    try {
        bool x = b.test(8);
        std::cout << x << '\n';
    } catch (const std::out_of_range&) {
        std::cout << "test(8) threw out_of_range (checked access)\n";
    }

    // In range, both work; use operator[] when the index is known good.
    std::cout << "b.test(3) = " << std::boolalpha << b.test(3) << '\n';
    std::cout << "b[3]      = " << b[3] << '\n';
}
