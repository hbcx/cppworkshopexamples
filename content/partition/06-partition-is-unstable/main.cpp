#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
    auto isEven = [](int x) { return x % 2 == 0; };

    // BAD: relying on the elements KEEPING their relative order within each group
    // after std::partition. partition may reorder them freely -- the order within
    // the evens and within the odds is UNSPECIFIED, so code that expects exactly
    // 2 4 6 8 is not portable. (Not printed here, precisely because it is
    // unspecified and could differ between library implementations.)
    std::vector<int> bad = v;
    std::partition(bad.begin(), bad.end(), isEven);

    // GOOD: when the order within each group matters, use stable_partition, which
    // guarantees the evens stay 2 4 6 8 and the odds stay 1 3 5 7.
    std::vector<int> good = v;
    auto point = std::stable_partition(good.begin(), good.end(), isEven);

    std::cout << "stable evens: ";
    for (auto it = good.begin(); it != point; ++it) std::cout << *it << ' ';
    std::cout << "| odds: ";
    for (auto it = point; it != good.end(); ++it) std::cout << *it << ' ';
    std::cout << '\n';
    return 0;
}
