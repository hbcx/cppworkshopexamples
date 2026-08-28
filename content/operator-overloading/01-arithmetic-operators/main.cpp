#include <iostream>

// A small money type. The goal: make Money add like a built-in number, so the
// reader can write a + b and a += b and get exactly what they expect.

class Money {
public:
    explicit Money(long cents = 0) : cents_(cents) {}

    long cents() const { return cents_; }

    // Compound assignment is the primitive. It MUTATES this object and returns a
    // reference to it, so assignments can chain: a += b += c.
    Money& operator+=(const Money& rhs) {
        cents_ += rhs.cents_;
        return *this;
    }

private:
    long cents_;
};

// Binary + is built ON TOP of +=. It must NOT change its operands, so it takes a
// copy of the left side, adds the right into it, and returns the copy by value.
// Free function (not a member) so that both operands are treated the same -- see
// the member-vs-free example for why this matters for symmetry.
Money operator+(Money lhs, const Money& rhs) {
    lhs += rhs;          // reuse the member; no logic duplicated
    return lhs;          // return the modified copy by value
}

int main() {
    Money a{500};        // 5.00
    Money b{250};        // 2.50
    Money c{125};        // 1.25

    Money sum = a + b + c;
    std::cout << "a + b + c = " << sum.cents() << " cents\n";   // 875

    // + did not touch its operands:
    std::cout << "a still = " << a.cents() << " cents\n";       // 500

    // += mutates and chains, right to left:
    Money d{0};
    d += a += b;         // a becomes 750, then d becomes 750
    std::cout << "after d += a += b: a = " << a.cents()
              << ", d = " << d.cents() << "\n";                 // 750, 750
    return 0;
}
