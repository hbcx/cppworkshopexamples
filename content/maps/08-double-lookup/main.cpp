#include <iostream>
#include <map>
#include <string>

// A comparator that counts how many key comparisons the map performs. Every tree
// lookup costs a handful of these, so doing the lookup twice doubles the work.
struct CountingLess {
    static long calls;
    bool operator()(const std::string& a, const std::string& b) const {
        ++calls;
        return a < b;
    }
};
long CountingLess::calls = 0;

using Map = std::map<std::string, int, CountingLess>;

int main() {
    Map m;
    for (int i = 0; i < 8; ++i) {
        m.emplace(std::string(1, static_cast<char>('a' + i)), i);
    }

    // --- anti-pattern: two lookups to answer one question ("value of d, or 0") ---
    CountingLess::calls = 0;
    int bad = 0;
    if (m.count("d")) {          // lookup #1
        bad = m.at("d");         // lookup #2 -- walks the same path again
    }
    const long badCalls = CountingLess::calls;

    // --- fix: one lookup, reuse the iterator ---
    CountingLess::calls = 0;
    int good = 0;
    Map::const_iterator it = m.find("d");   // the only lookup
    if (it != m.end()) {
        good = it->second;
    }
    const long goodCalls = CountingLess::calls;

    std::cout << "same result: bad=" << bad << " good=" << good << '\n';
    std::cout << "key comparisons: two-lookup=" << badCalls
              << " one-lookup=" << goodCalls << '\n';
    std::cout << "two-lookup did more comparisons? "
              << (badCalls > goodCalls ? "yes" : "no") << '\n';
    return 0;
}
