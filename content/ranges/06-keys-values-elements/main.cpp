#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <tuple>
#include <vector>

int main() {
    std::map<std::string, int> scores = {{"ann", 90}, {"bob", 75}, {"cy", 88}};

    // views::keys and views::values pull one side out of each pair.
    std::cout << "names:";
    for (const auto& k : scores | std::views::keys) std::cout << " " << k;   // ann bob cy
    std::cout << "\nscores:";
    for (int s : scores | std::views::values) std::cout << " " << s;         // 90 75 88
    std::cout << "\n";

    // views::elements<N> generalises that to the Nth field of any tuple-like element.
    std::vector<std::tuple<std::string, int, double>> rows = {
        {"x", 1, 1.5}, {"y", 2, 2.5}
    };
    std::cout << "column 2:";
    for (double d : rows | std::views::elements<2>) std::cout << " " << d;   // 1.5 2.5
    std::cout << "\n";
    return 0;
}
