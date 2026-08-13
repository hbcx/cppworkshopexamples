#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> a{1, 2, 3, 5};   // sorted
    std::vector<int> b{2, 3, 4};      // sorted

    // BAD: merge then unique + erase to build a union. merge keeps EVERY element
    // (2 and 3 appear in both, so twice), so it needs a second dedup pass to become
    // a union -- two operations and an extra buffer for what is one call.
    std::vector<int> merged;
    std::merge(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(merged));
    merged.erase(std::unique(merged.begin(), merged.end()), merged.end());

    // GOOD: set_union produces the deduplicated union directly, in one pass.
    std::vector<int> u;
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(u));

    std::cout << "merge+unique: ";
    for (int x : merged) std::cout << x << ' ';
    std::cout << "\nset_union:    ";
    for (int x : u) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
