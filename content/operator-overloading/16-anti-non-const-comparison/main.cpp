#include <iostream>

// Anti-pattern: operator== as a non-const member. It works for the simplest case
// and hides two problems that only appear later: const objects cannot be compared,
// and the comparison is not symmetric.

class BadPrice {
public:
    BadPrice(int cents) : cents_(cents) {}     // implicit on purpose

    // BAD: non-const member. Fine for p == q on mutable objects, but:
    //   * a const BadPrice cannot call it  -> const cp; cp == q  does NOT compile
    //   * only the LEFT-hand member is found -> 100 == p          does NOT compile
    bool operator==(const BadPrice& other) { return cents_ == other.cents_; }

private:
    int cents_;
};

class GoodPrice {
public:
    GoodPrice(int cents) : cents_(cents) {}

    // FIX: free friend taking two const& operands. Callable on const objects, and
    // symmetric because both sides go through the same conversions.
    friend bool operator==(const GoodPrice& a, const GoodPrice& b) {
        return a.cents_ == b.cents_;
    }

private:
    int cents_;
};

int main() {
    BadPrice p{100}, q{100};
    std::cout << "bad  p == q: " << (p == q) << "\n";   // 1  (this much works)

    const GoodPrice cp{250};
    GoodPrice r{250};

    std::cout << "good cp == r:  " << (cp == r)   << "\n";  // 1  (const is fine)
    std::cout << "good r == 250: " << (r == 250)  << "\n";  // 1  (right side converts)
    std::cout << "good 250 == r: " << (250 == r)  << "\n";  // 1  (LEFT side converts too)
    return 0;
}
