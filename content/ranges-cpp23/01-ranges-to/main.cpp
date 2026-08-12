#include <ranges>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <iostream>

int main() {
    std::vector<int> nums{1, 2, 3, 4, 5, 6};

    // Materialize a lazy pipeline into a real container in ONE step. Before C++23
    // this was a loop with push_back, or std::copy piece by piece through a
    // back_inserter. ranges::to deduces the element type from the pipeline.
    auto evensDoubled = nums
        | std::views::filter([](int x){ return x % 2 == 0; })
        | std::views::transform([](int x){ return x * 10; })
        | std::ranges::to<std::vector>();

    std::cout << "to<vector>: ";
    for (int x : evensDoubled) std::cout << x << ' ';
    std::cout << '\n';

    // The target does not have to be a vector: collect chars into a string.
    auto letters = std::views::iota('a', 'f') | std::ranges::to<std::string>();
    std::cout << "to<string>: " << letters << '\n';

    // Build a map straight from a pipeline of pairs.
    auto squares = nums
        | std::views::transform([](int x){ return std::pair{x, x * x}; })
        | std::ranges::to<std::map<int, int>>();
    std::cout << "to<map>: ";
    for (auto [k, v] : squares) std::cout << k << "->" << v << ' ';
    std::cout << '\n';
    return 0;
}
