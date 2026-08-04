// A lazy generator: an iterator with no container behind it at all.
//
// FibUpTo yields the Fibonacci numbers below a limit, computed one at a time as
// the loop advances. Nothing is stored: the iterator holds the two running
// values and produces the next on ++, so an unbounded idea (the Fibonacci
// sequence) is consumed as a finite range without ever materialising it. This is
// the purest form of the pattern -- traversal completely decoupled from storage,
// so the same range-based for that walks a vector walks a computation.

#include <iostream>

class FibUpTo {
    unsigned limit_;
public:
    explicit FibUpTo(unsigned limit) : limit_(limit) {}

    class iterator {
        unsigned a_, b_;   // a_ is the current value; b_ the one after
        unsigned limit_;
        bool done_;
    public:
        iterator(unsigned a, unsigned b, unsigned limit, bool done)
            : a_(a), b_(b), limit_(limit), done_(done) {}

        unsigned operator*() const { return a_; }

        iterator& operator++() {
            unsigned next = a_ + b_;
            a_ = b_;
            b_ = next;
            if (a_ >= limit_) done_ = true; // the generator decides its own end
            return *this;
        }

        // A sentinel-style comparison: a live iterator differs from end() until
        // it marks itself done. This is the common shape for a generator, whose
        // end is a CONDITION reached while computing, not a stored position.
        bool operator!=(const iterator& other) const { return done_ != other.done_; }
    };

    iterator begin() const {
        return iterator(0u, 1u, limit_, limit_ == 0u); // 0 is the first Fibonacci number
    }
    iterator end() const {
        return iterator(0u, 0u, limit_, true);
    }
};

int main() {
    // Range-based for over a computed sequence -- no elements exist anywhere.
    std::cout << "fib < 50:";
    for (unsigned x : FibUpTo(50))
        std::cout << " " << x;
    std::cout << "\n";

    std::cout << "fib < 5: ";
    for (unsigned x : FibUpTo(5))
        std::cout << " " << x;
    std::cout << "\n";
    return 0;
}
