#include <algorithm>
#include <iostream>
#include <iterator>
#include <unordered_set>
#include <vector>

int main() {
    // 20 values; only the very last one is in the needle set, so the scan does
    // the most work before it finds a match.
    std::vector<int> haystack;
    for (int i = 100; i < 119; ++i) haystack.push_back(i);
    haystack.push_back(7);            // the only match, at the end

    std::vector<int> needles;         // a large set of candidates: 1..50
    for (int i = 1; i <= 50; ++i) needles.push_back(i);

    // ANTI-PATTERN: std::find_first_of against a big needle set. For every
    // haystack element it walks the WHOLE needle list until a match, so the cost
    // is O(n * m) -- here up to 20 * 50 comparisons. We count them with a
    // predicate to make the cost visible.
    long comparisons = 0;
    auto found = std::find_first_of(haystack.begin(), haystack.end(),
                                    needles.begin(), needles.end(),
                                    [&](int h, int n) { ++comparisons; return h == n; });
    std::cout << "find_first_of found " << *found
              << " after " << comparisons << " element comparisons\n";

    // FIX: put the needles in a hash set once, then scan the haystack ONCE,
    // testing membership in O(1). Cost drops to O(n + m).
    std::unordered_set<int> lookup(needles.begin(), needles.end());
    long scanned = 0;
    auto hit = std::find_if(haystack.begin(), haystack.end(),
                            [&](int h) { ++scanned; return lookup.count(h) != 0; });
    std::cout << "hash-set scan visited: " << scanned << " elements\n";
    std::cout << "both found value: " << *hit << '\n';
    return 0;
}
