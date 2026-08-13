#include <algorithm>
#include <set>
#include <iostream>

int main() {
    std::set<int> s{1, 3, 5, 7, 9, 11, 13};

    // BAD: std::lower_bound works on a std::set (it is sorted), but a set has only
    // BIDIRECTIONAL iterators, so the binary search cannot jump to the middle in
    // O(1) -- it must step one node at a time, making the whole thing O(n) even
    // though it performs O(log n) comparisons.
    auto viaAlgo = std::lower_bound(s.begin(), s.end(), 7);

    // GOOD: the set's MEMBER lower_bound (and find) walk the tree in O(log n),
    // using the structure a std::algorithm cannot see through the iterators.
    auto viaMember = s.lower_bound(7);

    std::cout << "algorithm found: " << *viaAlgo
              << ", member found: " << *viaMember << '\n';
    return 0;
}
