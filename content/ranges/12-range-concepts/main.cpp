#include <iostream>
#include <list>
#include <ranges>
#include <vector>

int main() {
    // ranges::begin/end/size are customization points that work on containers,
    // arrays and views alike.
    int raw[] = {10, 20, 30};
    std::cout << "array size via ranges::size = " << std::ranges::size(raw) << "\n";   // 3
    std::cout << "first via ranges::begin = " << *std::ranges::begin(raw) << "\n";     // 10

    // The concepts classify what a range can do.
    static_assert(std::ranges::contiguous_range<std::vector<int>>);
    static_assert(std::ranges::random_access_range<std::vector<int>>);
    static_assert(std::ranges::sized_range<std::vector<int>>);
    static_assert(std::ranges::bidirectional_range<std::list<int>>);
    static_assert(!std::ranges::random_access_range<std::list<int>>);

    // A view is a lightweight range; a container is a range but not a view.
    auto evens = std::views::iota(1, 10) | std::views::filter([](int x){ return x % 2 == 0; });
    static_assert(std::ranges::view<decltype(evens)>);
    static_assert(std::ranges::range<std::vector<int>>);
    static_assert(!std::ranges::view<std::vector<int>>);

    std::cout << "all range concepts hold\n";
    return 0;
}
