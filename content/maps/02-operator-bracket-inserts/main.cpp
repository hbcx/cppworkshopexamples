#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> counts;
    counts["apples"] = 3;

    // operator[] with a missing key INSERTS a value-initialised entry (0 for int)
    // and returns a reference to it -- so this "read" silently grows the map.
    std::cout << "lookup bananas via []: " << counts["bananas"] << '\n';
    std::cout << "size after that lookup: " << counts.size() << '\n';   // now 2!
    std::cout << "map contains bananas now? "
              << (counts.find("bananas") != counts.end() ? "yes" : "no") << '\n';

    // To look WITHOUT inserting, use find (returns end() when absent).
    auto it = counts.find("cherries");
    std::cout << "cherries present? " << (it != counts.end() ? "yes" : "no")
              << " -- size still " << counts.size() << '\n';

    // The convenient side of the same behaviour: a word-count builds itself.
    std::map<std::string, int> freq;
    const char* words[] = {"a", "b", "a", "a", "b"};
    for (const char* w : words) {
        ++freq[w];   // [] creates the entry at 0 on first sight, then increments
    }
    std::cout << "freq a=" << freq["a"] << " b=" << freq["b"] << '\n';
    return 0;
}
