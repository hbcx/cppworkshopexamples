#include <iostream>

// Anti-pattern: operator+ that mutates the left operand. It compiles, and simple
// tests may pass, but a + b now has a side effect on a -- the opposite of what
// everyone assumes + does.

class BadVec {
public:
    explicit BadVec(int x) : x_(x) {}
    int x() const { return x_; }

    // BAD: this adds rhs INTO *this and returns a reference to *this. So a + b
    // changes a, and the "result" is just a again. (Returning a reference to a
    // fresh local instead would be even worse -- it would dangle.)
    BadVec& operator+(const BadVec& rhs) {
        x_ += rhs.x_;
        return *this;
    }

private:
    int x_;
};

class GoodVec {
public:
    explicit GoodVec(int x) : x_(x) {}
    int x() const { return x_; }

    // The mutating primitive is +=, and it is honestly named.
    GoodVec& operator+=(const GoodVec& rhs) { x_ += rhs.x_; return *this; }

private:
    int x_;
};

// FIX: + is non-mutating, works on a copy, returns a new value.
GoodVec operator+(GoodVec lhs, const GoodVec& rhs) { lhs += rhs; return lhs; }

int main() {
    BadVec a{10}, b{5};
    BadVec sum = a + b;
    std::cout << "bad:  sum = " << sum.x()
              << ", but a is now " << a.x() << "\n";   // sum = 15, a is now 15 (corrupted)

    GoodVec c{10}, d{5};
    GoodVec good = c + d;
    std::cout << "good: sum = " << good.x()
              << ", and c is still " << c.x() << "\n"; // sum = 15, and c is still 10
    return 0;
}
