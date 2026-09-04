#include <iostream>
#include <iterator>
#include <list>

int main() {
    std::list<int> lst{1, 2, 3, 4, 5, 6};

    // std::advance moves an iterator IN PLACE by n steps (no return value). It is
    // the in-place counterpart of std::next, and the way to step a non-random-
    // access iterator -- a list has no it + n.
    auto it = lst.begin();
    std::advance(it, 3);
    std::cout << "after advance(it, 3):  *it = " << *it << "\n";   // 4

    // n may be NEGATIVE for a bidirectional iterator -- step backward in place.
    std::advance(it, -2);
    std::cout << "after advance(it, -2): *it = " << *it << "\n";   // 2

    // std::next keeps the original and returns a moved copy; std::advance moves
    // the iterator you already hold. Same stepping, different ownership.
    auto ahead = std::next(it, 2);   // it stays put; ahead is 2 further on
    std::cout << "*it = " << *it << ", *ahead = " << *ahead << "\n";   // 2, 4
    return 0;
}
