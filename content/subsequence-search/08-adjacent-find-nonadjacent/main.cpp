#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

int main() {
    std::cout << std::boolalpha;

    // The value 1 appears twice, but the two 1s are NOT next to each other.
    std::vector<int> data{1, 2, 3, 1, 4};

    // ANTI-PATTERN: use adjacent_find to answer "does this range have any
    // duplicate?". adjacent_find only inspects NEIGHBOURS, so it misses the two
    // 1s that are separated -- and reports "no duplicates", which is wrong.
    bool noAdjacentDup = std::adjacent_find(data.begin(), data.end()) == data.end();
    std::cout << "adjacent_find says no duplicates? " << noAdjacentDup
              << "   <- misleading\n";

    // FIX 1: sort a copy first so equal values become neighbours; THEN
    // adjacent_find (or std::unique) sees them.
    std::vector<int> sorted = data;
    std::sort(sorted.begin(), sorted.end());
    bool hasDupAfterSort =
        std::adjacent_find(sorted.begin(), sorted.end()) != sorted.end();
    std::cout << "after sorting, duplicate found? " << hasDupAfterSort << '\n';

    // FIX 2: track seen values in a hash set -- one pass, no sorting, and it
    // reports the first repeat regardless of position.
    std::unordered_set<int> seen;
    bool anyDup = false;
    for (int x : data) {
        if (!seen.insert(x).second) { anyDup = true; break; }
    }
    std::cout << "hash-set says any duplicate? " << anyDup << '\n';
    return 0;
}
