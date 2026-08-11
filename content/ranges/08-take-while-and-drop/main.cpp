#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> v = {2, 4, 6, 7, 8, 10};

    // take_while yields the leading run that satisfies the predicate, then stops at
    // the first that does not (7 here) -- even though 8 and 10 would also match.
    std::cout << "take_while even:";
    for (int x : v | std::views::take_while([](int x){ return x % 2 == 0; }))
        std::cout << " " << x;                          // 2 4 6
    std::cout << "\n";

    // drop skips the first n; drop_while skips the leading run that matches.
    std::cout << "drop 2:";
    for (int x : v | std::views::drop(2)) std::cout << " " << x;   // 6 7 8 10
    std::cout << "\n";

    std::cout << "drop_while even:";
    for (int x : v | std::views::drop_while([](int x){ return x % 2 == 0; }))
        std::cout << " " << x;                          // 7 8 10
    std::cout << "\n";
    return 0;
}
