#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main() {
    std::vector<int> v1{1, 2, 3, 100, 5, 6};
    std::vector<int> v2{1, 2, 3, 200, 5, 6};

    // std::mismatch walks both ranges together and stops at the FIRST position
    // where they differ, returning a pair of iterators -- one into each range --
    // pointing at the differing elements.
    auto diff = std::mismatch(v1.begin(), v1.end(), v2.begin());

    // The distance from the start to the first iterator is the length of the
    // common prefix (how far the ranges agree before splitting).
    std::cout << "common prefix length: "
              << std::distance(v1.begin(), diff.first) << '\n';
    std::cout << "first difference: " << *diff.first
              << " vs " << *diff.second << '\n';

    // A common use: how much of a path or key two strings share.
    std::string p1 = "/usr/local/bin";
    std::string p2 = "/usr/local/lib";
    auto pd = std::mismatch(p1.begin(), p1.end(), p2.begin());
    std::cout << "shared path prefix: "
              << std::string(p1.begin(), pd.first) << '\n';

    // Caution carried to the anti-patterns: if the ranges are EQUAL, mismatch
    // returns the end iterators, and dereferencing them then is out of bounds --
    // always compare against last before reading *diff.first.
    return 0;
}
