#include <ranges>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{10, 20, 30, 40};

    // slide(n): a sliding window of width n that moves one step at a time -- the
    // windows OVERLAP (unlike chunk). The number of windows is size - n + 1.
    std::cout << "slide(2):\n";
    for (auto win : v | std::views::slide(2)) {
        std::cout << "  ";
        for (int x : win) std::cout << x << ' ';
        std::cout << '\n';
    }

    // adjacent<N>: the same fixed-width sliding window, but N is a compile-time
    // constant and each window is a TUPLE, so you can name the parts. adjacent<2>
    // is also spelled pairwise.
    std::cout << "adjacent<2> deltas: ";
    for (auto [a, b] : v | std::views::adjacent<2>)
        std::cout << (b - a) << ' ';
    std::cout << '\n';
    return 0;
}
