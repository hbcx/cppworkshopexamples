#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::cout << std::boolalpha;

    // The logical family wraps &&, || and !.
    std::logical_and<bool> AND;
    std::logical_or<bool> OR;
    std::logical_not<bool> NOT;
    std::cout << "AND(true, false) = " << AND(true, false) << "\n";   // false
    std::cout << "OR(true, false)  = " << OR(true, false) << "\n";    // true
    std::cout << "NOT(false)       = " << NOT(false) << "\n";         // true

    // Reduce a range of flags to "are they all set?" with std::logical_and.
    std::vector<bool> flags{true, true, false, true};
    bool allSet = std::accumulate(flags.begin(), flags.end(), true,
                                  std::logical_and<bool>());
    std::cout << "all set? " << allSet << "\n";   // false

    // The bitwise family wraps &, | and ^. A running XOR over a range with
    // std::bit_xor is a simple checksum.
    std::vector<unsigned> data{0x01u, 0xF0u, 0x0Fu, 0xFFu};
    unsigned checksum = std::accumulate(data.begin(), data.end(), 0u,
                                        std::bit_xor<unsigned>());
    std::cout << "xor checksum = " << checksum << "\n";   // 1

    // std::bit_and folds & across the range (bit_not is the C++14 addition).
    unsigned common = std::accumulate(data.begin(), data.end(), 0xFFu,
                                      std::bit_and<unsigned>());
    std::cout << "and-fold = " << common << "\n";   // 0
    return 0;
}
