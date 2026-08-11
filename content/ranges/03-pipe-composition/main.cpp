#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Adaptors compose with the pipe operator, left to right, reading like a
    // sentence: keep the even numbers, square them, take the first three.
    auto pipe = v | std::views::filter([](int x){ return x % 2 == 0; })
                  | std::views::transform([](int x){ return x * x; })
                  | std::views::take(3);

    std::cout << "even, squared, first three:";
    for (int x : pipe) std::cout << " " << x;   // 4 16 36
    std::cout << "\n";
    return 0;
}
