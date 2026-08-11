#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector<std::vector<int>> groups = {{1, 2}, {3, 4, 5}, {6}};

    // views::join flattens a range of ranges into one range, lazily -- no copy into
    // a combined vector.
    std::cout << "flattened:";
    for (int x : groups | std::views::join) std::cout << " " << x;   // 1 2 3 4 5 6
    std::cout << "\n";

    // It composes: flatten, then keep the even ones.
    std::cout << "flattened evens:";
    for (int x : groups | std::views::join
                        | std::views::filter([](int x){ return x % 2 == 0; }))
        std::cout << " " << x;                                       // 2 4 6
    std::cout << "\n";
    return 0;
}
