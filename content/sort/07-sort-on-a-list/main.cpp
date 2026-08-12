#include <algorithm>
#include <list>
#include <iostream>

int main() {
    std::list<int> items{5, 2, 8, 1, 9};

    // std::sort needs RANDOM-ACCESS iterators -- it jumps around and takes
    // midpoints -- but a std::list only offers bidirectional iterators. So this
    // does not even compile:
    //
    //   std::sort(items.begin(), items.end());   // error: list iterators are not random-access
    //
    // The BAD here is a COMPILE error, not a runtime bug -- the type system stops you.

    // GOOD: std::list has its own member sort. It relinks nodes instead of moving
    // values, so it works with the list's bidirectional iterators.
    items.sort();
    std::cout << "list sorted (member sort): ";
    for (int x : items) std::cout << x << ' ';
    std::cout << '\n';

    // The same holds for std::forward_list::sort. And you never sort a std::set or
    // std::map: they keep their keys ordered for you, and the keys are const.
    return 0;
}
