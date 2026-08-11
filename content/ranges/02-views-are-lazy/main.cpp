#include <iostream>
#include <ranges>
#include <vector>

int calls = 0;

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

    // Building a view computes NOTHING. transform runs only when an element is
    // actually consumed -- and take stops the range early.
    auto squares = v | std::views::transform([](int x){ ++calls; return x * x; });

    std::cout << "after building the view, calls = " << calls << "\n";   // 0

    std::cout << "first 3 squares:";
    for (int x : squares | std::views::take(3)) std::cout << " " << x;    // 1 4 9
    std::cout << "\n";

    std::cout << "transform ran " << calls << " times (only 3, not 8)\n"; // 3
    return 0;
}
