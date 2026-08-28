#include <iostream>
#include <compare>
#include <string>

// Text is ordered lexicographically by its content. We write <=> by hand, so ==
// is NOT generated -- we add it, and make it take the shortcut that ordering
// cannot: strings of different length are unequal after a single size check.

struct Text {
    std::string data;

    // Hand-written ordering. std::string's own <=> is strong_ordering.
    std::strong_ordering operator<=>(const Text& other) const {
        return data <=> other.data;
    }

    // Required separately (a custom <=> does not synthesize ==). The size check
    // answers "unequal" with no character comparison at all.
    bool operator==(const Text& other) const {
        if (data.size() != other.data.size()) return false;   // fast path
        return data == other.data;
    }
};

int main() {
    Text a{"hello"};
    Text b{"hello"};
    Text c{"help"};
    Text d{"hi"};

    std::cout << "a == b? " << (a == b) << "\n";   // 1
    std::cout << "a == d? " << (a == d) << "\n";   // 0  (decided by length, 5 != 2)
    std::cout << "a <  c? " << (a < c)  << "\n";   // 1  (hello < help)
    std::cout << "c >  a? " << (c > a)  << "\n";   // 1

    // Without the operator== above, "a == b" would not even compile: a hand-written
    // <=> gives the relational operators, never equality.
    return 0;
}
