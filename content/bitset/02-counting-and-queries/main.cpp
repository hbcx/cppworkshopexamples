// count() is how many bits are set (a population count, often one instruction),
// size() is the fixed length N, and any/all/none answer the common questions in
// one call.

#include <bitset>
#include <iostream>

int main() {
    std::bitset<12> b("101100101000");

    std::cout << "bits:  " << b << '\n';
    std::cout << "count: " << b.count() << " set out of " << b.size() << '\n';

    std::cout << std::boolalpha;
    std::cout << "any set?  " << b.any()  << '\n';
    std::cout << "all set?  " << b.all()  << '\n';
    std::cout << "none set? " << b.none() << '\n';

    std::bitset<12> empty;
    std::cout << "empty.none()? " << empty.none() << '\n';
}
