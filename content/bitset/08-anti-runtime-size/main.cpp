// Anti-pattern: trying to size a bitset from a run-time value.
//
// std::bitset<N> bakes N into the type, so N must be a compile-time constant.
// When the length is only known at run time, std::vector<bool> packs bits and
// sizes dynamically (boost::dynamic_bitset if you also want the bitwise ops).

#include <bitset>
#include <iostream>
#include <vector>

int main() {
    std::size_t n = 5;   // known only at run time (imagine it came from input)

    // BAD (does not compile -- described):
    //   std::bitset<n> bits;   // N must be a constant expression, n is not

    // GOOD: vector<bool> is a run-time-sized packed bit array.
    std::vector<bool> bits(n, false);
    bits[1] = true;
    bits[3] = true;

    std::cout << "vector<bool> of size " << bits.size() << ": ";
    for (std::size_t i = 0; i < bits.size(); ++i) std::cout << (bits[i] ? '1' : '0');
    std::cout << '\n';

    std::size_t set = 0;
    for (std::size_t i = 0; i < bits.size(); ++i) if (bits[i]) ++set;
    std::cout << "set bits: " << set << '\n';
}
