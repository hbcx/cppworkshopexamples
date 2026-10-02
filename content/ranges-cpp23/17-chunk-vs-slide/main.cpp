#include <ranges>
#include <vector>
#include <iostream>

// chunk and slide look alike but do opposite things. chunk(n) cuts the range into
// NON-overlapping blocks; slide(n) produces OVERLAPPING windows. Reaching for the
// wrong one gives the wrong groups and the wrong count.

int main() {
    std::vector<int> temps{20, 21, 23, 22, 25};

    // WRONG for a moving average: chunk(3) gives disjoint blocks, so 22 and 25
    // land in a separate short block and no window ever covers 23, 22, 25.
    std::cout << "chunk(3) blocks (non-overlapping):\n";
    for (auto block : temps | std::views::chunk(3)) {
        std::cout << "  ";
        for (int x : block) std::cout << x << ' ';
        std::cout << '\n';
    }

    // RIGHT for a moving average: slide(3) gives every consecutive window.
    std::cout << "slide(3) windows (overlapping):\n";
    for (auto win : temps | std::views::slide(3)) {
        std::cout << "  ";
        for (int x : win) std::cout << x << ' ';
        std::cout << '\n';
    }
    return 0;
}
