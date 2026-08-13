#include <numeric>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};

    // inclusive_scan is partial_sum's modern sibling: out[i] INCLUDES in[i] (a
    // running sum), but its operation may be applied in ANY order, so it can run in
    // parallel. Same result as partial_sum for a plain sum.
    std::vector<int> inc;
    std::inclusive_scan(v.begin(), v.end(), std::back_inserter(inc));
    std::cout << "inclusive_scan: ";
    for (int x : inc) std::cout << x << ' ';
    std::cout << '\n';

    // exclusive_scan EXCLUDES the current element: out[i] is the sum of everything
    // BEFORE i, starting from an init value. So out[0] is the init (0 here), and
    // each entry is the running sum shifted by one.
    std::vector<int> exc;
    std::exclusive_scan(v.begin(), v.end(), std::back_inserter(exc), 0);
    std::cout << "exclusive_scan: ";
    for (int x : exc) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
