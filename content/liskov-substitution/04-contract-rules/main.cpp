// The contract rules that keep substitution safe, and their direction.
//
// Base Discount's contract:
//   precondition:  n >= 0
//   postcondition: result is in [0, n]
//
// A subtype stays substitutable if it WEAKENS the precondition (accepts more)
// and STRENGTHENS the postcondition (promises more). Capped does both: it also
// handles n < 0, and it guarantees a tighter range [0, n/5], which is still
// inside the base's [0, n]. The reverse moves are the violations shown later.

#include <iostream>

class Discount {
public:
    virtual ~Discount() {}
    // precondition: n >= 0 ; postcondition: 0 <= result <= n
    virtual long apply(long n) const = 0;
};

// Honours the contract exactly: 10% off, result in [0, n].
class Standard : public Discount {
public:
    long apply(long n) const override { return n / 10; }
};

// Still substitutable:
//  - weaker precondition: also defined for n < 0 (returns 0)
//  - stronger postcondition: result in [0, n/5], a subset of [0, n]
class Capped : public Discount {
public:
    long apply(long n) const override {
        if (n < 0) return 0;
        long ten = n / 10;
        long cap = n / 5;
        return ten < cap ? ten : cap;
    }
};

// Written to the BASE contract: relies only on result being in [0, n], so the
// price it returns is never negative and never above n.
long priceAfter(const Discount& d, long n) {
    return n - d.apply(n);
}

int main() {
    Standard standard;
    Capped capped;

    std::cout << "standard 1000 -> " << priceAfter(standard, 1000) << "\n";
    std::cout << "capped   1000 -> " << priceAfter(capped, 1000) << "\n";

    // Both keep the caller's guarantee; either can stand in for Discount.
    std::cout << "capped honours [0,n] even for a big n: 500 -> "
              << priceAfter(capped, 500) << "\n";
}
