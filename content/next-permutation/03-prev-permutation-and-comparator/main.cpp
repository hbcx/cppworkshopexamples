#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

int main() {
    // prev_permutation steps the OTHER way -- to the previous permutation in
    // lexicographic order. From 3 2 1 (the largest) the previous is 3 1 2.
    std::vector<int> v{3, 2, 1};
    std::prev_permutation(v.begin(), v.end());
    std::cout << "prev of 3 2 1: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // Both take an optional comparator, which defines the order permutations step
    // through. With std::greater, "next" walks in descending order, so to
    // enumerate all permutations that way start from the largest (sorted
    // descending) range.
    std::vector<int> d{3, 2, 1};
    int count = 0;
    do {
        ++count;
    } while (std::next_permutation(d.begin(), d.end(), std::greater<int>()));
    std::cout << "descending-order enumeration visited " << count
              << " permutations\n";
    return 0;
}
