#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> ids{5, 2, 5, 1, 2, 5, 3, 1};

    // To remove ALL duplicates, not just adjacent ones, sort first so equal values
    // become neighbours, then unique + erase collapses every run. sort + unique +
    // erase is the standard idiom for deduplicating a vector in place.
    std::sort(ids.begin(), ids.end());
    ids.erase(std::unique(ids.begin(), ids.end()), ids.end());

    std::cout << "distinct ids (sorted): ";
    for (int x : ids) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
