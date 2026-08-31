// Build a flat container from a whole range: the constructor sorts once in
// O(n log n). If the input is ALREADY sorted and unique, pass std::sorted_unique
// so it skips the sort for an O(n) build. Inserting one at a time is O(n^2).

#include <flat_map>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main() {
    // Unsorted input: the range constructor sorts it once.
    std::vector<std::pair<int, std::string>> rows{
        {3, "gamma"}, {1, "alpha"}, {2, "beta"}};
    std::flat_map<int, std::string> built(rows.begin(), rows.end());

    std::cout << "range-built (sorted for us):\n";
    for (auto [k, v] : built) std::cout << "  " << k << " -> " << v << '\n';

    // Already sorted and unique (say, from ORDER BY): skip the sort.
    std::vector<std::pair<int, std::string>> ordered{
        {1, "alpha"}, {2, "beta"}, {3, "gamma"}};
    std::flat_map<int, std::string> fast(std::sorted_unique,
                                         ordered.begin(), ordered.end());

    std::cout << "sorted_unique-built (no sort): size " << fast.size()
              << ", first " << fast.begin()->first << '\n';
    std::cout << "both equal? " << std::boolalpha << (built == fast) << '\n';
}
