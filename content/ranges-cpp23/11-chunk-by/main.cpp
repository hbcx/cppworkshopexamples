#include <ranges>
#include <vector>
#include <functional>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 3, 3, 5, 6, 6};

    // chunk_by starts a new group whenever a binary predicate on two ADJACENT
    // elements is false. equal_to groups equal runs together.
    std::cout << "group equal runs:\n";
    for (auto group : v | std::views::chunk_by(std::ranges::equal_to{})) {
        std::cout << "  ";
        for (int x : group) std::cout << x << ' ';
        std::cout << '\n';
    }

    // A different predicate: keep an ascending run together, splitting where the
    // next element is not greater.
    std::vector<int> t{1, 3, 7, 2, 4, 1, 9};
    std::cout << "ascending runs:\n";
    for (auto group : t | std::views::chunk_by([](int a, int b){ return a < b; })) {
        std::cout << "  ";
        for (int x : group) std::cout << x << ' ';
        std::cout << '\n';
    }
    return 0;
}
