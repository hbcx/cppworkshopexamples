// Anti-pattern: erasing matching elements one at a time in a loop.
//
// A middle erase on a deque is O(n) and invalidates all iterators, so the loop
// is O(n^2) and steps a stale iterator after each erase. The erase-remove idiom
// packs survivors in one O(n) pass, then erases the tail once.

#include <algorithm>
#include <deque>
#include <iostream>

int main() {
    std::deque<int> d = {1, 2, 3, 4, 5, 6, 7, 8};

    // BAD (O(n^2) and undefined -- described, not run):
    //   for (auto it = d.begin(); it != d.end(); ++it)
    //       if (*it % 2 == 0) d.erase(it);   // erase invalidates it; ++it is UB,
    //                                        // and each erase shifts O(n) elements

    // GOOD: the erase-remove idiom -- one O(n) pass, one erase.
    d.erase(std::remove_if(d.begin(), d.end(),
                           [](int x) { return x % 2 == 0; }),
            d.end());

    std::cout << "odd numbers kept:";
    for (int x : d) std::cout << ' ' << x;
    std::cout << '\n';
}
