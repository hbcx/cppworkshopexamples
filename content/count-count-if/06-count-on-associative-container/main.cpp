#include <algorithm>
#include <set>
#include <iostream>

// A key type whose comparisons count themselves, to expose how much work each
// search does. std::count uses ==; the tree uses <.
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
    // A multiset with repeats: four 5s among other keys.
    std::multiset<Key> bag;
    for (int v : {1, 3, 5, 5, 5, 7, 9, 5}) bag.insert(Key{v});

    // BAD: std::count walks every element with == -- O(n), ignoring the tree.
    Key::eqCalls = 0;
    auto bad = std::count(bag.begin(), bag.end(), Key{5});
    std::cout << "std::count of 5: " << bad
              << "  (== comparisons: " << Key::eqCalls << ")\n";

    // GOOD: multiset's member count uses the tree -- O(log n + k), far fewer
    // comparisons, and the same answer.
    Key::ltCalls = 0;
    auto good = bag.count(Key{5});
    std::cout << "member count of 5: " << good
              << "  (< comparisons: " << Key::ltCalls << ")\n";
    return 0;
}
