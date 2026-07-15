#include <iostream>
#include <vector>

int main() {
    // std::vector<bool> is a SPECIAL CASE: it packs its elements into bits, so it
    // is not a normal container of bool. operator[] returns a proxy, not bool&.
    std::vector<bool> bits{true, false, true};

    // `auto` deduces the proxy type (std::vector<bool>::reference), NOT bool.
    // The proxy refers back into the vector, so it sees later writes -- a plain
    // `bool b = bits[0]` would instead have copied the value.
    auto b = bits[0];
    bits[0] = false;          // write through the vector...
    std::cout << "proxy tracked the write? " << (b == false ? "yes" : "no")
              << "  (auto captured a reference, not a copy)\n";

    // There is no data() and you cannot take &bits[i]: a bit is not addressable,
    // so vector<bool> cannot back a C API. A vector<char> can.
    std::vector<char> real{1, 0, 1};
    char* p = real.data();    // vector<char> has real, addressable storage
    std::cout << "vector<char> is addressable, first byte=" << static_cast<int>(p[0])
              << '\n';
    return 0;
}
