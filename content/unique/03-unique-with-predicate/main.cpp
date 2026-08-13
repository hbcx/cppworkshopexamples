#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#include <iostream>

// Are two strings equal ignoring case?
static bool iequal(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i])))
            return false;
    }
    return true;
}

int main() {
    std::vector<std::string> words{"Yes", "yes", "YES", "no", "No", "yes"};

    // unique takes an optional binary predicate that decides when two NEIGHBOURS
    // count as the same. Here, case-insensitive equality collapses a run of the
    // same word written with different capitalization to its first spelling.
    auto end = std::unique(words.begin(), words.end(), iequal);
    words.erase(end, words.end());

    // Yes/yes/YES collapse to Yes; no/No collapse to no; the final yes is a new
    // run (not adjacent to the first), so it stays.
    std::cout << "collapsed: ";
    for (const auto& w : words) std::cout << w << ' ';
    std::cout << '\n';
    return 0;
}
