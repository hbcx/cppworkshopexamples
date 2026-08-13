#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 1, 2, 2, 2, 3, 1, 1};

    // unique removes CONSECUTIVE equal elements only. Like remove, it does not
    // resize -- it moves the survivors to the front and returns the new logical
    // end. Pair it with erase to actually drop the tail.
    auto new_end = std::unique(v.begin(), v.end());
    v.erase(new_end, v.end());

    // The runs 1 1 / 2 2 2 / 3 each collapse to one, but the final 1 1 is a
    // SEPARATE run (not adjacent to the first), so a 1 appears again: unique only
    // compares neighbours, it does not deduplicate the whole range.
    std::cout << "after unique+erase: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
