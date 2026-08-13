#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> data{50, 12, 88, 7, 33, 91, 4, 60};   // large input, unsorted

    // partial_sort_copy writes the k smallest elements, in sorted order, into a
    // SEPARATE output whose SIZE decides k. The input is not modified -- useful when
    // it is const, or a stream you read once and only want the top few of.
    std::vector<int> top(3);   // want the 3 smallest
    std::partial_sort_copy(data.begin(), data.end(), top.begin(), top.end());

    std::cout << "3 smallest (copied out): ";
    for (int x : top) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
