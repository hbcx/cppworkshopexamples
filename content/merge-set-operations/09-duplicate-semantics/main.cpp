#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    // Inputs WITH duplicates -- these are multisets, not sets.
    std::vector<int> a{1, 1, 2, 3};   // sorted; two 1s
    std::vector<int> b{1, 2, 2, 4};   // sorted; two 2s

    // The set operations follow MULTISET rules; they do NOT force uniqueness.
    // set_intersection keeps min(count in a, count in b) of each value, and
    // set_union keeps max(...). So the intersection has one 1 (min(2,1)) and one 2
    // (min(1,2)); the union has two 1s (max(2,1)) and two 2s (max(1,2)).
    std::vector<int> inter, uni;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                          std::back_inserter(inter));
    std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                   std::back_inserter(uni));

    std::cout << "intersection: ";
    for (int x : inter) std::cout << x << ' ';
    std::cout << "\nunion:        ";
    for (int x : uni) std::cout << x << ' ';
    std::cout << '\n';

    // If you want a truly unique result, make the INPUTS unique first (sort +
    // unique), so every value appears at most once and the counts are 0 or 1.
    return 0;
}
