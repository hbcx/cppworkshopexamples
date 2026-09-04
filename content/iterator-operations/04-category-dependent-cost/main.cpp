#include <cstddef>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

// A generic helper: works on ANY iterator category thanks to the iterator
// operations, without caring whether it + n exists.
template <typename It>
auto elementAt(It begin, std::size_t n) -> decltype(*begin) {
    std::advance(begin, static_cast<std::ptrdiff_t>(n));
    return *begin;
}

int main() {
    std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::list<int>   l{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // The SAME code runs on both -- a uniform interface is the point.
    std::cout << "v element 7 = " << elementAt(v.begin(), 7) << "\n";   // 7
    std::cout << "l element 7 = " << elementAt(l.begin(), 7) << "\n";   // 7

    // But the COST differs by iterator category, and the library picks the right
    // implementation for each (see the tag-dispatch chapter):
    //   * vector's random-access iterator: advance/next/distance are O(1) -- a
    //     single pointer computation (begin + n).
    //   * list's bidirectional iterator: they are O(n) -- there is no jump, so the
    //     library steps one node at a time.
    // So elementAt(v, 7) is one addition, while elementAt(l, 7) walks 7 nodes.
    std::cout << "advance/distance: O(1) on a vector, O(n) on a list\n";
    return 0;
}
