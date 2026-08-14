#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{10, 20, 30, 40, 50};

    // iter_swap swaps the ELEMENTS that two iterators point at -- it dereferences
    // both and swaps the pointed-to values. Swap the first and last.
    std::iter_swap(v.begin(), v.end() - 1);
    std::cout << "after swapping ends: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // It is the building block the rearranging algorithms use internally: a
    // hand-written reverse is just iter_swap of the two ends, working inward.
    std::vector<int> w{1, 2, 3, 4, 5, 6};
    for (auto lo = w.begin(), hi = w.end(); lo != hi && lo != --hi; ++lo)
        std::iter_swap(lo, hi);
    std::cout << "hand-rolled reverse: ";
    for (int x : w) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
