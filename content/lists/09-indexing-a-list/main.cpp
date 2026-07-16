#include <cstddef>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>

namespace {

// std::next written out longhand, so the pointer hops can be counted. This is
// exactly what the real std::next does for a bidirectional iterator: it has no
// way to jump, so it follows one link at a time.
long long hops = 0;

std::list<int>::const_iterator counted_next(std::list<int>::const_iterator it,
                                            std::size_t n) {
    for (std::size_t k = 0; k < n; ++k) {
        ++it;
        ++hops;
    }
    return it;
}

}  // namespace

int main() {
    const std::size_t n = 1000;
    std::list<int> l(n);
    std::iota(l.begin(), l.end(), 1);

    // Anti-pattern: it reads exactly like a vector's index loop, and it is
    // quadratic. Every iteration starts again from begin() and walks i links, so
    // visiting n elements costs 0 + 1 + 2 + ... + (n-1) hops.
    hops = 0;
    long long sum = 0;
    for (std::size_t i = 0; i < l.size(); ++i) {
        sum += *counted_next(l.begin(), i);   // == *std::next(l.begin(), i)
    }
    std::cout << "std::next indexing: sum=" << sum << " in " << hops << " pointer hops\n";
    const long long quadratic = hops;

    // The fix: keep the iterator. It already knows where it is, so each element
    // costs exactly one hop. Same result, linear cost.
    hops = 0;
    sum = 0;
    for (auto it = l.begin(); it != l.end(); ++it) {
        sum += *it;
        ++hops;
    }
    std::cout << "plain iteration:    sum=" << sum << " in " << hops << " pointer hops\n";
    std::cout << "ratio: " << quadratic / hops << "x more work for the same answer, at n="
              << n << '\n';
    std::cout << "and it grows with n: the indexed loop is O(n^2), the iterator loop O(n)\n";
    return 0;
}
