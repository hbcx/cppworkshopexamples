#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7};
    const std::size_t k = 3;   // left-rotate by 3

    // BAD: hand-rolling a rotation with a temporary buffer -- save the first k, shift
    // the rest down, copy the saved block to the end. It ALLOCATES, the shift bounds
    // are easy to get wrong, and it is far more code than the one call.
    std::vector<int> bad = v;
    std::vector<int> temp(bad.begin(), bad.begin() + k);      // extra allocation
    for (std::size_t i = k; i < bad.size(); ++i)
        bad[i - k] = bad[i];
    std::copy(temp.begin(), temp.end(), bad.end() - k);

    // GOOD: std::rotate does exactly this IN PLACE, with no temporary buffer and no
    // manual index arithmetic.
    std::vector<int> good = v;
    std::rotate(good.begin(), good.begin() + k, good.end());

    std::cout << "hand-rolled: ";
    for (int x : bad) std::cout << x << ' ';
    std::cout << '\n';
    std::cout << "std::rotate: ";
    for (int x : good) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
