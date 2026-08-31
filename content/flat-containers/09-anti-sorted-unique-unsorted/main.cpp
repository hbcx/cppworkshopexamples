// Anti-pattern: passing sorted_unique with data that is not sorted and unique.
//
// The tag is a promise the constructor trusts without checking: it skips the
// sort and dedup. Break the promise and the sorted invariant is violated, so
// binary-search lookups become undefined. Only use it on genuinely sorted,
// unique input.

#include <flat_set>
#include <iostream>
#include <vector>

int main() {
    // BAD (undefined -- described, not run):
    //   std::vector<int> messy{5, 1, 3, 1};         // NOT sorted, has a duplicate
    //   std::flat_set<int> broken(std::sorted_unique, messy.begin(), messy.end());
    //   broken.contains(3);   // invariant violated: lookup may miss a present key

    // GOOD 1: plain range constructor -- it sorts and de-duplicates for you.
    std::vector<int> messy{5, 1, 3, 1};
    std::flat_set<int> safe(messy.begin(), messy.end());
    std::cout << "range-built (sorted, deduped):";
    for (int x : safe) std::cout << ' ' << x;
    std::cout << "  contains(3)? " << std::boolalpha << safe.contains(3) << '\n';

    // GOOD 2: sorted_unique, but only because this input really is sorted + unique.
    std::vector<int> ordered{1, 3, 5};
    std::flat_set<int> fast(std::sorted_unique, ordered.begin(), ordered.end());
    std::cout << "sorted_unique-built:";
    for (int x : fast) std::cout << ' ' << x;
    std::cout << "  contains(3)? " << fast.contains(3) << '\n';
}
