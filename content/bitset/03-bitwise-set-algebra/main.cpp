// Read bit i as "element i is in the set" and the bitwise operators become set
// operations, each running on whole machine words at once.

#include <bitset>
#include <iostream>

int main() {
    std::bitset<8> a("00110101");   // elements {0, 2, 4, 5}
    std::bitset<8> b("01011100");   // elements {2, 3, 4, 6}

    std::cout << "A            = " << a << '\n';
    std::cout << "B            = " << b << '\n';
    std::cout << "A & B (both) = " << (a & b) << '\n';   // intersection {2,4}
    std::cout << "A | B (either)= " << (a | b) << '\n';  // union
    std::cout << "A ^ B (one)  = " << (a ^ b) << '\n';   // symmetric difference
    std::cout << "~A (complement)= " << (~a) << '\n';    // everything not in A
    std::cout << "A << 1 (shift up)= " << (a << 1) << '\n';

    std::cout << "elements in both: " << (a & b).count() << '\n';
}
