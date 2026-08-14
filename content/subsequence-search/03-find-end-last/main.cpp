#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    // The needle {1,2} occurs three times: at indices 0, 2, and 5.
    std::vector<int> haystack{1, 2, 1, 2, 5, 1, 2};
    std::vector<int> needle{1, 2};

    // std::search finds the FIRST occurrence...
    auto first = std::search(haystack.begin(), haystack.end(),
                             needle.begin(), needle.end());
    std::cout << "search   (first) at index "
              << std::distance(haystack.begin(), first) << '\n';

    // ...and std::find_end finds the LAST occurrence of the same subsequence.
    // Same arguments, opposite end -- despite the name it is a subsequence search,
    // not a single-element find.
    auto last = std::find_end(haystack.begin(), haystack.end(),
                              needle.begin(), needle.end());
    std::cout << "find_end (last)  at index "
              << std::distance(haystack.begin(), last) << '\n';

    // Like the others it returns end when the needle is absent, so check first.
    // A common use: locate the LAST separator or record marker in a buffer.
    return 0;
}
