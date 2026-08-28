#include <iostream>
#include <compare>

// Anti-pattern: a hand-written <=> and no ==. The relational operators work, so it
// looks complete -- until someone writes a == b and the build fails, because a
// user-provided <=> does not generate equality.

struct Broken {
    int n;
    // Ordering only. This does NOT declare operator==.
    std::strong_ordering operator<=>(const Broken& other) const {
        return n <=> other.n;
    }
    // With just the above, "a == b" is a COMPILE ERROR -- ordering exists,
    // equality does not.
};

struct Fixed {
    int n;
    std::strong_ordering operator<=>(const Fixed& other) const {
        return n <=> other.n;
    }
    // THE FIX: declare == too. Defaulted memberwise equality is right here.
    bool operator==(const Fixed&) const = default;
};

int main() {
    Broken b1{3}, b2{5};
    // Relational operators are available from the custom <=>:
    std::cout << "Broken: b1 < b2? " << (b1 < b2) << "\n";   // 1

    Fixed f1{3}, f2{3}, f3{5};
    // Now BOTH ordering and equality work:
    std::cout << "Fixed:  f1 == f2? " << (f1 == f2) << "\n";  // 1
    std::cout << "Fixed:  f1 != f3? " << (f1 != f3) << "\n";  // 1
    std::cout << "Fixed:  f1 <  f3? " << (f1 < f3)  << "\n";  // 1
    return 0;
}
