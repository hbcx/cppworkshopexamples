#include <algorithm>
#include <set>
#include <iostream>

// A key type whose comparisons count themselves, to expose how much work each
// search does. std::find uses ==; the set's tree uses <.
struct Key {
    int v;
    static int eqCalls;
    static int ltCalls;
    bool operator==(const Key& o) const { ++eqCalls; return v == o.v; }
    bool operator<(const Key& o) const { ++ltCalls; return v < o.v; }
};
int Key::eqCalls = 0;
int Key::ltCalls = 0;

int main() {
    std::set<Key> ids;
    for (int i = 1; i <= 8; ++i) ids.insert(Key{i});

    // BAD: std::find ignores the set's ordering and walks element by element with
    // == -- an O(n) linear scan that does not use the tree at all.
    Key::eqCalls = 0;
    auto bad = std::find(ids.begin(), ids.end(), Key{8});
    std::cout << "std::find found 8? " << std::boolalpha << (bad != ids.end())
              << "  (== comparisons: " << Key::eqCalls << ")\n";

    // GOOD: the member .find() descends the tree with < -- O(log n), far fewer
    // comparisons, and it is the same answer.
    Key::ltCalls = 0;
    auto good = ids.find(Key{8});
    std::cout << "member find found 8? " << (good != ids.end())
              << "  (< comparisons: " << Key::ltCalls << ")\n";
    return 0;
}
