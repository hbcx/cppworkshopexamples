// std::unordered_set answers one question fast: have I seen this value before?
// It stores unique elements in a hash table, so insert and membership are O(1)
// on average. The classic use is deduplication -- a "seen" set that a stream of
// items is checked against as it arrives.

#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

int main() {
    const char* stream[] = {"login", "click", "login", "purchase", "click", "login"};
    const std::size_t count = sizeof(stream) / sizeof(stream[0]);

    std::unordered_set<std::string> seen;
    std::vector<std::string> firstSeen;  // arrival order, first occurrence only

    for (std::size_t i = 0; i < count; ++i) {
        // insert returns {iterator, bool}. The bool is true only the first time
        // this value is added -- so it both stores the item and tells you whether
        // it was new, in a single hash lookup.
        const bool isNew = seen.insert(stream[i]).second;
        std::cout << stream[i] << (isNew ? "  (new)\n" : "  (seen before)\n");
        if (isNew) {
            firstSeen.push_back(stream[i]);
        }
    }

    std::cout << "distinct values: " << seen.size() << "\n";

    // Membership test after the fact: find (iterator or end()) or count (0/1).
    std::cout << "seen 'purchase'? " << (seen.count("purchase") ? "yes" : "no") << "\n";
    std::cout << "seen 'logout'?   "
              << (seen.find("logout") != seen.end() ? "yes" : "no") << "\n";

    std::cout << "first occurrences, in arrival order:";
    for (std::size_t i = 0; i < firstSeen.size(); ++i) {
        std::cout << " " << firstSeen[i];
    }
    std::cout << "\n";
    return 0;
}
