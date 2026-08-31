// Anti-pattern: iterating a bitset like a container.
//
// bitset has no begin()/end(), so range-for and standard algorithms do not
// compile over it. Walk it by index, and prefer the members (count, to_string,
// any/none/all) for the common jobs.

#include <bitset>
#include <iostream>

int main() {
    std::bitset<8> b("10110010");

    // BAD (does not compile -- described):
    //   for (bool bit : b) { ... }            // no begin()/end()
    //   std::count(b.begin(), b.end(), true); // same reason

    // GOOD: walk by index, bounded by size().
    std::cout << "set bit positions:";
    for (std::size_t i = 0; i < b.size(); ++i)
        if (b[i]) std::cout << ' ' << i;
    std::cout << '\n';

    // GOOD: the common queries are members -- no loop needed.
    std::cout << "count(): " << b.count() << '\n';
    std::cout << "to_string(): " << b.to_string() << '\n';
}
