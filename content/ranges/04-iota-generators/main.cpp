#include <iostream>
#include <ranges>

int main() {
    // iota(n) is a lazy, INFINITE sequence n, n+1, n+2, ... Bound it with take.
    std::cout << "first 5 from 1:";
    for (int x : std::views::iota(1) | std::views::take(5)) std::cout << " " << x;   // 1..5
    std::cout << "\n";

    // iota(first, last) is the finite half-open range [first, last).
    std::cout << "iota(10, 15):";
    for (int x : std::views::iota(10, 15)) std::cout << " " << x;   // 10 11 12 13 14
    std::cout << "\n";

    // A generator feeding a pipeline: squares of 1..5 with no container at all.
    std::cout << "squares 1..5:";
    for (int x : std::views::iota(1, 6) | std::views::transform([](int n){ return n * n; }))
        std::cout << " " << x;                                      // 1 4 9 16 25
    std::cout << "\n";
    return 0;
}
