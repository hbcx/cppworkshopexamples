// Anti-pattern: a custom iterator that skips its priming step.
//
// A filtering iterator (yield only elements passing a predicate) has a classic
// bug: it advances correctly in ++ but forgets to skip disallowed elements when
// it is FIRST created, so the very first element it yields may be one that
// should have been filtered out. EvenRange below yields even numbers; the buggy
// begin() hands back the raw first element without skipping, so an odd leading
// element leaks through. The fix is to prime the iterator on construction so it
// starts on a valid element -- the same skip logic ++ already uses.

#include <iostream>
#include <cstddef>

class EvenRange {
    const int* data_;
    std::size_t n_;
public:
    EvenRange(const int* data, std::size_t n) : data_(data), n_(n) {}

    class iterator {
        const int* p_;
        const int* end_;
        void skipOdd() { while (p_ != end_ && *p_ % 2 != 0) ++p_; }
    public:
        // prime == true runs the skip so the iterator starts on an even element.
        iterator(const int* p, const int* end, bool prime) : p_(p), end_(end) {
            if (prime) skipOdd();
        }
        int operator*() const { return *p_; }
        iterator& operator++() { ++p_; skipOdd(); return *this; }
        bool operator!=(const iterator& o) const { return p_ != o.p_; }
    };

    // BUG: hands back the first element without skipping to the first even one.
    iterator beginBad()  const { return iterator(data_, data_ + n_, false); }
    // Correct: prime on construction, exactly as ++ maintains the invariant.
    iterator beginGood() const { return iterator(data_, data_ + n_, true); }
    iterator end()       const { return iterator(data_ + n_, data_ + n_, false); }
};

int main() {
    const int nums[] = { 1, 2, 3, 4, 5 }; // evens are 2 and 4
    EvenRange r(nums, sizeof nums / sizeof nums[0]);

    std::cout << "bad: ";
    for (auto it = r.beginBad(); it != r.end(); ++it)
        std::cout << " " << *it;     // leaks the leading odd 1
    std::cout << "\n";

    std::cout << "good:";
    for (auto it = r.beginGood(); it != r.end(); ++it)
        std::cout << " " << *it;     // 2 and 4 only
    std::cout << "\n";
    return 0;
}
