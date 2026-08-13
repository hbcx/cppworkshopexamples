#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> a{3, 1, 2};   // NOT sorted
    std::vector<int> b{2, 4, 1};   // NOT sorted

    // BAD: set_intersection (like merge and every set operation) requires BOTH
    // inputs to be sorted. On unsorted ranges it walks them assuming order and
    // produces a wrong result -- the behavior is undefined, not just inaccurate.
    // Shown only as a comment:
    //   std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), out);

    // GOOD: sort both inputs first, by the SAME order, then the set operation is
    // correct. The common elements of {1,2,3} and {1,2,4} are 1 and 2.
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    std::vector<int> both;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                          std::back_inserter(both));
    std::cout << "intersection after sorting: ";
    for (int x : both) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
