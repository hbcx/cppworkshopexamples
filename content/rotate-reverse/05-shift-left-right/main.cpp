#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    // std::shift_left (C++20) moves elements toward the front by n. The elements
    // that fall off the front are gone; the n slots at the BACK are left in a
    // valid-but-unspecified (moved-from) state. It returns the new end of the
    // meaningful data, so print only up to there -- never the moved-from tail.
    std::vector<int> a{1, 2, 3, 4, 5, 6};
    auto newEnd = std::shift_left(a.begin(), a.end(), 2);
    std::cout << "shift_left by 2:";
    for (auto it = a.begin(); it != newEnd; ++it) std::cout << ' ' << *it;
    std::cout << "   (" << (newEnd - a.begin()) << " valid elements)\n";

    // std::shift_right moves elements toward the back by n. Now the front n slots
    // are the moved-from ones, and it returns the new BEGIN of the valid data.
    std::vector<int> b{1, 2, 3, 4, 5, 6};
    auto newBegin = std::shift_right(b.begin(), b.end(), 2);
    std::cout << "shift_right by 2:";
    for (auto it = newBegin; it != b.end(); ++it) std::cout << ' ' << *it;
    std::cout << '\n';

    // Unlike rotate, shift DROPS the elements pushed off the end rather than
    // wrapping them around -- see the shift-vs-rotate anti-pattern.
    return 0;
}
