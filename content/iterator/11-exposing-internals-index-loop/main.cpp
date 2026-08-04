// Anti-pattern: exposing internals instead of offering iterators.
//
// The Iterator pattern exists so callers can traverse a container without
// knowing how it stores its elements. Skip it -- expose the raw storage (a data
// pointer and a size, or an operator[] tied to contiguous layout) -- and every
// caller couples to that representation: they index into an array, so the array
// can never become a list without breaking them all. Offer begin()/end() instead
// and callers use range-for and algorithms, oblivious to the layout, which is
// then free to change.

#include <iostream>
#include <cstddef>

// ---- BAD: leaks its representation; callers must know it is an array ----
class StackBad {
    int data_[8];
    std::size_t n_ = 0;
public:
    void push(int v) { data_[n_++] = v; }
    const int* data() const { return data_; } // hands out the raw storage
    std::size_t size() const { return n_; }
};

// A caller is now coupled to "it is a contiguous array indexed 0..size-1".
static int sumBad(const StackBad& s) {
    int sum = 0;
    for (std::size_t i = 0; i < s.size(); ++i)
        sum += s.data()[i]; // reaches into the representation
    return sum;
}

// ---- GOOD: offers iterators; callers never see the representation ----
class StackGood {
    int data_[8];
    std::size_t n_ = 0;
public:
    void push(int v) { data_[n_++] = v; }
    // The iterator happens to be a pointer here, but callers do not know or care;
    // if the storage became a linked list, only these two lines would change.
    const int* begin() const { return data_; }
    const int* end()   const { return data_ + n_; }
};

// The caller is coupled only to "it is iterable" -- nothing about the layout.
static int sumGood(const StackGood& s) {
    int sum = 0;
    for (int v : s) sum += v; // range-for; works for any representation
    return sum;
}

int main() {
    StackBad b;
    for (int v : { 1, 2, 3, 4 }) b.push(v);
    std::cout << "bad sum:  " << sumBad(b) << "\n";

    StackGood g;
    for (int v : { 1, 2, 3, 4 }) g.push(v);
    std::cout << "good sum: " << sumGood(g) << "\n";
    return 0;
}
