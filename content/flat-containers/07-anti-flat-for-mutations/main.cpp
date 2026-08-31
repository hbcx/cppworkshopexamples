// Anti-pattern: filling or mutating a flat container one element at a time.
//
// Each insert shifts the vector to stay sorted (O(n)), so n single inserts cost
// O(n^2). Build from a whole range instead (one O(n log n) sort). For genuinely
// mutation-heavy use, a node-based std::map or hashed std::unordered_map fits.

#include <flat_map>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main() {
    // BAD shape: build by repeated single inserts -- O(n^2), each shifts the vector.
    std::flat_map<int, std::string> slow;
    for (int i = 5; i >= 1; --i) slow.insert({i, "v" + std::to_string(i)});

    // GOOD: hand the whole range to the constructor -- sorts once.
    std::vector<std::pair<int, std::string>> rows;
    for (int i = 5; i >= 1; --i) rows.push_back({i, "v" + std::to_string(i)});
    std::flat_map<int, std::string> fast(rows.begin(), rows.end());

    std::cout << "same contents, different cost to build:\n";
    std::cout << "  slow (n inserts, O(n^2)): size " << slow.size() << '\n';
    std::cout << "  fast (one range build):   size " << fast.size() << '\n';
    std::cout << "  equal? " << std::boolalpha << (slow == fast) << '\n';
    std::cout << "for scattered inserts and erases over time, use std::map instead\n";
}
