#include <algorithm>
#include <vector>
#include <iterator>
#include <functional>
#include <iostream>

int main() {
    std::vector<int> a{1, 2, 3, 4};
    std::vector<int> b{10, 20, 30, 40};

    // The binary form takes TWO input ranges and a two-argument function, combining
    // them element by element: result[i] = op(a[i], b[i]).
    std::vector<int> sums;
    std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(sums),
        std::plus<int>{});
    std::cout << "a + b:     ";
    for (int x : sums) std::cout << x << ' ';
    std::cout << '\n';

    // Any two-argument rule works -- a lambda for a weighted combine.
    std::vector<int> weighted;
    std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(weighted),
        [](int x, int y){ return x * 2 + y; });
    std::cout << "2*a + b:   ";
    for (int x : weighted) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
