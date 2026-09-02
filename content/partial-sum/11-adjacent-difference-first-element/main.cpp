#include <numeric>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{5, 8, 12, 20};

    // adjacent_difference writes n elements, the SAME count as the input -- NOT
    // n-1. The FIRST output element is a COPY of the first input (v[0]), not a
    // difference; only from index 1 on are the values real differences.
    std::vector<int> d;
    std::adjacent_difference(v.begin(), v.end(), std::back_inserter(d));

    std::cout << "output size: " << d.size() << " (input size " << v.size() << ")\n";
    std::cout << "values: ";
    for (int x : d) std::cout << x << ' ';
    std::cout << '\n';

    // So d[0] is 5 (the original), not 0. Code that assumes the first entry is a
    // gap, or that the result is one shorter, is wrong. To use only the true
    // differences, skip the first element (start at d.begin() + 1).
    std::cout << "true differences (skip first): ";
    for (auto it = d.begin() + 1; it != d.end(); ++it) std::cout << *it << ' ';
    std::cout << '\n';
    return 0;
}
