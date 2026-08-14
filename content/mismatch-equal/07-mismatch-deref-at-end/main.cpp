#include <algorithm>
#include <iostream>
#include <vector>

// Report the first differing element of two equal-length ranges -- safely.
static void reportFirstDiff(const std::vector<int>& a, const std::vector<int>& b) {
    auto d = std::mismatch(a.begin(), a.end(), b.begin(), b.end());

    // ANTI-PATTERN: read *d.first without checking. When the ranges are EQUAL
    // (or the first is a prefix of the second), mismatch returns a.end(), and
    //   std::cout << *d.first;
    // dereferences the end iterator -- out of bounds, undefined behaviour.

    // FIX: compare against end() first. If d.first reached the end, there was no
    // difference; only then is *d.first a real element.
    if (d.first == a.end()) {
        std::cout << "ranges are equal\n";
    } else {
        std::cout << "first difference at index " << (d.first - a.begin())
                  << ": " << *d.first << " vs " << *d.second << '\n';
    }
}

int main() {
    reportFirstDiff({1, 2, 3, 4}, {1, 2, 9, 4});   // differ at index 2
    reportFirstDiff({5, 6, 7}, {5, 6, 7});          // equal -> must NOT deref
    return 0;
}
