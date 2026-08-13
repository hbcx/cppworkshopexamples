#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{9, 7, 5, 3, 1};   // sorted DESCENDING (by std::greater)
    std::cout << std::boolalpha;

    // BAD: the range is sorted descending, but binary_search is called with the
    // DEFAULT (ascending) comparison. A binary search assumes the range is sorted
    // by the comparator it is GIVEN, so this mismatch searches as if the data were
    // ascending and reports 7 -- which is present -- as missing. Undefined behavior,
    // since the range is not sorted by the default order. Shown as a comment:
    //   std::binary_search(v.begin(), v.end(), 7);   // wrong comparator

    // GOOD: search with the SAME ordering the range was sorted by -- std::greater.
    bool found = std::binary_search(v.begin(), v.end(), 7, std::greater<int>());
    std::cout << "binary_search(7) with matching comparator: " << found << '\n';
    return 0;
}
