#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> data{4, 1, 7, 3, 9, 2};

    // BAD: reversing to get "descending order". reverse only flips the CURRENT
    // order -- on unsorted data that is just the data backwards, not sorted
    // descending at all.
    std::vector<int> bad = data;
    std::reverse(bad.begin(), bad.end());
    std::cout << "reverse of unsorted: ";
    for (int x : bad) std::cout << x << ' ';
    std::cout << '\n';

    // GOOD: to sort descending, sort with a greater-than comparator. (Sorting
    // ascending and then reversing also works, but one pass with std::greater is
    // clearer and does not depend on a prior sort.)
    std::vector<int> good = data;
    std::sort(good.begin(), good.end(), std::greater<int>());
    std::cout << "sorted descending:   ";
    for (int x : good) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
