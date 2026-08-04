// Anti-pattern: claiming an iterator category you do not honour.
//
// The iterator_category typedef is a promise the algorithm library trusts and
// dispatches on. Declare random_access_iterator_tag and the library will use the
// random-access operations -- std::advance calls it += n in a single step rather
// than looping ++ n times. If your += does not actually move n positions, that
// single call lands on the wrong element, silently. The bug is in the operator,
// but it only bites because the category promised the fast path.

#include <iostream>
#include <iterator> // std::advance
#include <cstddef>  // std::ptrdiff_t

// ---- BAD: tagged random-access, but += is a stub that steps only once ----
class BadIter {
    int* p_;
public:
    using iterator_category = std::random_access_iterator_tag; // the promise...
    using value_type        = int;
    using difference_type   = std::ptrdiff_t;
    using pointer           = int*;
    using reference         = int&;

    explicit BadIter(int* p) : p_(p) {}
    reference operator*() const { return *p_; }
    BadIter& operator++() { ++p_; return *this; }
    BadIter& operator--() { --p_; return *this; }
    BadIter& operator+=(difference_type) { ++p_; return *this; } // ...broken: ignores n
    BadIter& operator-=(difference_type n) { p_ -= n; return *this; }
    BadIter operator+(difference_type n) const { BadIter t = *this; t += n; return t; }
    difference_type operator-(const BadIter& o) const { return p_ - o.p_; }
    bool operator==(const BadIter& o) const { return p_ == o.p_; }
    bool operator!=(const BadIter& o) const { return p_ != o.p_; }
};

// ---- GOOD: += actually moves n positions, honouring the random-access claim ----
class GoodIter {
    int* p_;
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = int;
    using difference_type   = std::ptrdiff_t;
    using pointer           = int*;
    using reference         = int&;

    explicit GoodIter(int* p) : p_(p) {}
    reference operator*() const { return *p_; }
    GoodIter& operator++() { ++p_; return *this; }
    GoodIter& operator--() { --p_; return *this; }
    GoodIter& operator+=(difference_type n) { p_ += n; return *this; } // moves n
    GoodIter& operator-=(difference_type n) { p_ -= n; return *this; }
    GoodIter operator+(difference_type n) const { GoodIter t = *this; t += n; return t; }
    difference_type operator-(const GoodIter& o) const { return p_ - o.p_; }
    bool operator==(const GoodIter& o) const { return p_ == o.p_; }
    bool operator!=(const GoodIter& o) const { return p_ != o.p_; }
};

int main() {
    int a[] = { 10, 20, 30, 40, 50 };

    // std::advance sees random_access_iterator_tag and does a single it += 3.
    BadIter bad(a);
    std::advance(bad, 3);
    std::cout << "bad  advance(it,3): " << *bad << " (wanted 40)\n"; // lands on 20

    GoodIter good(a);
    std::advance(good, 3);
    std::cout << "good advance(it,3): " << *good << "\n";            // 40

    // Note: had the iterator honestly been tagged a FORWARD iterator (with only
    // ++), std::advance would have looped ++ three times and landed correctly.
    // The wrong result comes from claiming random-access and not delivering it.
    return 0;
}
