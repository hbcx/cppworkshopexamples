#include <algorithm>
#include <set>
#include <unordered_set>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> data{5, 2, 5, 1, 2, 5, 3, 1};

    // sort + unique is the right tool for a ONE-SHOT dedup of a batch you already
    // have: in place, cache-friendly, and it leaves a sorted vector.
    std::vector<int> viaSortUnique = data;
    std::sort(viaSortUnique.begin(), viaSortUnique.end());
    viaSortUnique.erase(
        std::unique(viaSortUnique.begin(), viaSortUnique.end()),
        viaSortUnique.end());

    // BAD when you are maintaining uniqueness as elements ARRIVE: re-running
    // sort + unique after every insert is O(n log n) each time. A std::set keeps
    // elements distinct BY CONSTRUCTION -- one O(log n) insert per element, always
    // deduplicated, always ordered.
    std::set<int> live(data.begin(), data.end());

    // std::unordered_set is the same idea with O(1) average inserts when you do
    // not need any order (its iteration order is unspecified, so only the count
    // is printed here).
    std::unordered_set<int> hashed(data.begin(), data.end());

    std::cout << "sort+unique: ";
    for (int x : viaSortUnique) std::cout << x << ' ';
    std::cout << '\n';
    std::cout << "std::set:    ";
    for (int x : live) std::cout << x << ' ';
    std::cout << '\n';
    std::cout << "unordered_set distinct count: " << hashed.size() << '\n';
    return 0;
}
