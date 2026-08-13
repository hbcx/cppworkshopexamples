#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    // BAD: fill_n / generate_n write COUNT elements starting at the iterator, with
    // no bounds check. On an EMPTY (or too-small) container, begin() has no room,
    // so the writes go past the end -- undefined behavior. Shown as a comment:
    //   std::vector<int> empty;
    //   std::fill_n(empty.begin(), 5, 0);   // writes 5 ints into no space: UB

    // GOOD, option 1: size the container first, then fill_n writes into real slots.
    std::vector<int> sized(5);
    std::fill_n(sized.begin(), 5, 0);

    // GOOD, option 2: a back_inserter turns each write into a push_back, so the
    // container grows to fit -- no pre-sizing needed.
    std::vector<int> grown;
    std::fill_n(std::back_inserter(grown), 5, 0);

    std::cout << "sized: " << sized.size() << ", grown: " << grown.size() << '\n';
    return 0;
}
