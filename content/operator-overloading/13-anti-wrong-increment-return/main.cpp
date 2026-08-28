#include <iostream>

// Anti-pattern: a postfix ++ that returns the NEW value. It compiles and the
// object does advance, but x++ no longer means "give me the old value, then
// advance" -- it now behaves exactly like ++x.

class BadCounter {
public:
    explicit BadCounter(int v) : v_(v) {}
    int value() const { return v_; }

    BadCounter& operator++() { ++v_; return *this; }         // prefix: fine

    // BAD postfix: increments first and returns the current (new) state.
    BadCounter operator++(int) { ++v_; return *this; }       // should return the OLD value

private:
    int v_;
};

class GoodCounter {
public:
    explicit GoodCounter(int v) : v_(v) {}
    int value() const { return v_; }

    // Prefix: advance, return *this BY REFERENCE.
    GoodCounter& operator++() { ++v_; return *this; }

    // Postfix: save the old state, advance, return the OLD state by value.
    GoodCounter operator++(int) { GoodCounter old = *this; ++v_; return old; }

private:
    int v_;
};

int main() {
    BadCounter b{5};
    int bad = (b++).value();      // expected 5 (the old value), got the new one
    std::cout << "bad  b++ returned " << bad
              << ", b is now " << b.value() << "\n";   // returned 6, b is now 6  (wrong)

    GoodCounter g{5};
    int good = (g++).value();     // correct: the value before the increment
    std::cout << "good g++ returned " << good
              << ", g is now " << g.value() << "\n";   // returned 5, g is now 6
    return 0;
}
