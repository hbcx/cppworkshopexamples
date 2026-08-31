// std::bitset<N> is N bits: set turns one on, reset off, flip inverts, and test
// or operator[] reads. With no index, set/reset/flip act on every bit. operator<<
// prints the highest bit on the left, so bit 0 is the rightmost character.

#include <bitset>
#include <iostream>

int main() {
    std::bitset<8> b;                 // 00000000

    b.set(0);                         // bit 0 on  -> 00000001
    b.set(3);                         // bit 3 on  -> 00001001
    b.flip(1);                        // invert 1  -> 00001011
    b[7] = true;                      // bit 7 on  -> 10001011
    std::cout << "bits: " << b << '\n';

    std::cout << "bit 3 set? " << std::boolalpha << b.test(3) << '\n';
    b.reset(0);                       // bit 0 off -> 10001010
    std::cout << "after reset(0): " << b << '\n';

    b.set();                          // all on
    std::cout << "all set:   " << b << '\n';
    b.reset();                        // all off
    std::cout << "all reset: " << b << '\n';
}
