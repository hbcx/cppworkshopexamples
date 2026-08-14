#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> data{1, 2, 2, 3, 4, 4, 5};

    // std::adjacent_find finds the first place two NEIGHBOURING elements are
    // equal -- the first repeated pair. It returns an iterator to the first of
    // the two, or end if no two neighbours are equal.
    auto dup = std::adjacent_find(data.begin(), data.end());
    if (dup != data.end()) {
        std::cout << "first equal neighbours: " << *dup << " at index "
                  << std::distance(data.begin(), dup) << '\n';
    }

    // With a predicate it finds the first neighbouring PAIR satisfying a relation.
    // Here: the first descent, where the next value is smaller than the current --
    // the point a rising sequence first turns down.
    std::vector<int> series{3, 5, 8, 8, 6, 7};
    auto drop = std::adjacent_find(series.begin(), series.end(),
                                   [](int a, int b) { return b < a; });
    std::cout << "first descent after index "
              << std::distance(series.begin(), drop) << " (value " << *drop << ")\n";

    // Note: adjacent_find only sees NEIGHBOURS. It finds duplicates that sit next
    // to each other, not duplicates scattered through the range (an anti-pattern).
    return 0;
}
