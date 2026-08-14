#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> haystack{5, 1, 2, 3, 2, 3, 4, 9};
    std::vector<int> needle{2, 3};

    // std::search finds the FIRST place where the whole needle appears as a
    // contiguous subsequence of the haystack, in order. It returns an iterator to
    // the start of that match, or haystack.end() if the needle is not found.
    auto it = std::search(haystack.begin(), haystack.end(),
                          needle.begin(), needle.end());

    if (it != haystack.end()) {
        std::cout << "found {2,3} at index "
                  << std::distance(haystack.begin(), it) << '\n';
    } else {
        std::cout << "not found\n";
    }

    // A needle that is not present returns end -- always check before using it.
    std::vector<int> missing{9, 9};
    auto none = std::search(haystack.begin(), haystack.end(),
                            missing.begin(), missing.end());
    std::cout << "{9,9} present? " << std::boolalpha
              << (none != haystack.end()) << '\n';

    // This is the generalization of substring search to any range: std::search
    // over two strings is exactly "find this text inside that text".
    return 0;
}
