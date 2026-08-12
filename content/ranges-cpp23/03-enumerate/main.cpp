#include <ranges>
#include <vector>
#include <string>
#include <iostream>

int main() {
    std::vector<std::string> steps{"mix", "bake", "cool", "serve"};

    // enumerate pairs each element with its position, counting from 0. No manual
    // counter to declare, increment, and keep in sync with the loop.
    for (auto [i, step] : std::views::enumerate(steps))
        std::cout << i << ": " << step << '\n';
    return 0;
}
