#include <ranges>
#include <iostream>

int main() {
    // repeat(value, n): a range that yields value n times, without storing n
    // copies. Good for filling, padding, or supplying a constant column.
    std::cout << "repeat(7, 3): ";
    for (int x : std::views::repeat(7, 3)) std::cout << x << ' ';
    std::cout << '\n';

    // repeat(value) with no count is INFINITE. Bind it with take. Nothing is
    // materialized until you iterate, so an infinite view is cheap to hold.
    std::cout << "repeat('*') | take(4): ";
    for (char c : std::views::repeat('*') | std::views::take(4)) std::cout << c;
    std::cout << '\n';
    return 0;
}
