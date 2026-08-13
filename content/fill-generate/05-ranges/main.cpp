#include <algorithm>
#include <ranges>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v(5);

    // ranges::fill takes the whole range directly, no begin/end pair.
    std::ranges::fill(v, 3);
    std::cout << "ranges::fill:     ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // ranges::generate takes the range and the nullary generator.
    int n = 0;
    std::ranges::generate(v, [&n]() { return n += 10; });
    std::cout << "ranges::generate: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
