#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 3, 5, 7, 9, 11};   // MUST be sorted

    // binary_search returns only a bool: is the value present? It halves the search
    // range at each step, so it is O(log n) -- but ONLY on a sorted range.
    std::cout << std::boolalpha;
    std::cout << "7 present? " << std::binary_search(v.begin(), v.end(), 7) << '\n';
    std::cout << "8 present? " << std::binary_search(v.begin(), v.end(), 8) << '\n';

    // std::find gives the same yes/no on ANY range, but scans linearly, O(n). Use
    // binary_search when the range is sorted and you only need membership.
    return 0;
}
