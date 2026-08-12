#include <ranges>
#include <vector>
#include <functional>
#include <iostream>

int main() {
    std::vector<int> a{1, 2, 3, 4};
    std::vector<int> b{10, 20, 30, 40};

    // zip_transform: like zip, but instead of yielding tuples it calls a function
    // on the matched elements and yields the results -- zip and transform at once.
    std::cout << "zip_transform (a*b): ";
    for (int p : std::views::zip_transform(std::multiplies{}, a, b))
        std::cout << p << ' ';
    std::cout << '\n';

    // adjacent_transform<N>: like adjacent<N>, but calls a function on each window
    // and yields the result. N=2 over b, with a subtraction, gives the deltas.
    std::cout << "adjacent_transform<2> (deltas of b): ";
    for (int d : b | std::views::adjacent_transform<2>([](int x, int y){ return y - x; }))
        std::cout << d << ' ';
    std::cout << '\n';
    return 0;
}
