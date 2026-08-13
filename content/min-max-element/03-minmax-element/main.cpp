#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{4, 1, 7, 1, 7, 3};

    // minmax_element finds BOTH extremes in a single pass, returning a pair of
    // iterators {min, max} -- cheaper than calling min_element and max_element
    // separately (about 3n/2 comparisons instead of 2n).
    auto mm = std::minmax_element(v.begin(), v.end());
    std::cout << "min: " << *mm.first << ", max: " << *mm.second << '\n';

    // The tie rule is asymmetric and worth knowing: minmax's min returns the FIRST
    // smallest element, but its max returns the LAST largest -- the opposite of a
    // standalone max_element, which returns the first. With two 1s and two 7s, the
    // min is the first 1 (index 1) and the max is the last 7 (index 4).
    std::cout << "min at index " << std::distance(v.begin(), mm.first)
              << ", max at index " << std::distance(v.begin(), mm.second) << '\n';
    return 0;
}
