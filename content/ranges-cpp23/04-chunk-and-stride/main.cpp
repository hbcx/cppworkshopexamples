#include <ranges>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7};

    // chunk(n): split into consecutive, NON-overlapping groups of up to n
    // elements. The last group is short when n does not divide the size evenly.
    std::cout << "chunk(3):\n";
    for (auto group : v | std::views::chunk(3)) {
        std::cout << "  ";
        for (int x : group) std::cout << x << ' ';
        std::cout << '\n';
    }

    // stride(n): keep every n-th element, starting from the first.
    std::cout << "stride(2): ";
    for (int x : v | std::views::stride(2)) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
