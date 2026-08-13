#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> all{1, 2, 3, 4, 5, 6};   // sorted
    std::vector<int> some{2, 4, 6};            // sorted

    // includes: is every element of the second range also in the first? A subset
    // test on two sorted ranges, in O(n + m), returning a bool.
    std::cout << std::boolalpha;
    std::cout << "all includes some? "
              << std::includes(all.begin(), all.end(), some.begin(), some.end())
              << '\n';

    // inplace_merge merges two CONSECUTIVE sorted sub-ranges of one container into
    // a single sorted range -- the merge step of merge sort. Here [0,3) is sorted
    // and [3,6) is sorted; inplace_merge stitches them into one sorted range.
    std::vector<int> v{1, 4, 7, 2, 3, 9};
    std::inplace_merge(v.begin(), v.begin() + 3, v.end());
    std::cout << "inplace_merge: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
