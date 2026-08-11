#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};

    auto evens_doubled = v | std::views::filter([](int x){ return x % 2 == 0; })
                           | std::views::transform([](int x){ return x * 2; });

    // A view is lazy and NOT a container: no storage, recomputes on each pass. To
    // keep a result -- to store, return, or reuse it -- copy its elements into a
    // real container. (C++23 adds std::ranges::to; in C++20, push in a loop.)
    std::vector<int> result;
    for (int x : evens_doubled) result.push_back(x);

    std::cout << "materialised:";
    for (int x : result) std::cout << " " << x;   // 4 8 12
    std::cout << "\n";
    std::cout << "result is a real vector of size " << result.size() << "\n";   // 3
    return 0;
}
