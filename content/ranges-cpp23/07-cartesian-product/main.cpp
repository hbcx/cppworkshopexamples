#include <ranges>
#include <vector>
#include <string>
#include <iostream>

int main() {
    std::vector<std::string> sizes{"S", "M", "L"};
    std::vector<std::string> colors{"red", "blue"};

    // cartesian_product yields every combination of one element from each range,
    // as a tuple. The first range varies slowest, the last fastest.
    for (auto [size, color] : std::views::cartesian_product(sizes, colors))
        std::cout << size << '-' << color << '\n';
    return 0;
}
