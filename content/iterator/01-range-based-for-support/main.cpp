// Iterator basics: make your own type work with range-based for.
//
// The GoF Iterator gives a way to traverse something without exposing how it is
// stored. In C++ that "something" plugs into the language itself: a type with
// begin() and end() returning an object that supports *it, ++it and it != end
// works in a range-based for loop and with the standard algorithms. IntRange
// below has no storage at all -- its iterator holds just the current integer and
// computes the sequence -- which shows an iterator need not point into memory.

#include <iostream>

class IntRange {
    int lo_, hi_; // half-open range [lo_, hi_)
public:
    IntRange(int lo, int hi) : lo_(lo), hi_(hi) {}

    // The minimal iterator: dereference, pre-increment, and inequality. That is
    // all a range-based for loop asks for.
    class iterator {
        int value_;
    public:
        explicit iterator(int v) : value_(v) {}
        int operator*() const { return value_; }
        iterator& operator++() { ++value_; return *this; }
        bool operator!=(const iterator& other) const { return value_ != other.value_; }
    };

    iterator begin() const { return iterator(lo_); }
    iterator end()   const { return iterator(hi_); }
};

int main() {
    // The loop calls begin()/end() once, then *it, ++it and it != end each pass.
    std::cout << "range [2, 7):";
    for (int x : IntRange(2, 7))
        std::cout << " " << x;
    std::cout << "\n";

    // Same object, driven by hand, to show what the loop expands to.
    std::cout << "by hand:   ";
    IntRange r(0, 4);
    for (IntRange::iterator it = r.begin(); it != r.end(); ++it)
        std::cout << " " << *it;
    std::cout << "\n";
    return 0;
}
