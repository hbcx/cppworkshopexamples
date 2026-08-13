#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

int main() {
    std::vector<int> src{1, 2, 3, 4, 5};

    // copy writes each element of a range to an output iterator. To a NEW container,
    // a back_inserter appends, so the output grows to fit.
    std::vector<int> grown;
    std::copy(src.begin(), src.end(), std::back_inserter(grown));
    std::cout << "into a new vector:   ";
    for (int x : grown) std::cout << x << ' ';
    std::cout << '\n';

    // Into a PRE-SIZED container, pass its begin -- copy overwrites the existing
    // elements. The destination must already have room for all of them.
    std::vector<int> sized(src.size());
    std::copy(src.begin(), src.end(), sized.begin());
    std::cout << "into a sized vector: ";
    for (int x : sized) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
