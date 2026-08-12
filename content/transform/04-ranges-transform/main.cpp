#include <algorithm>
#include <vector>
#include <string>
#include <iterator>
#include <ranges>
#include <iostream>

struct User {
    std::string name;
    int age;
};

int main() {
    std::vector<User> users{
        {"Ada", 36}, {"Linus", 28}, {"Grace", 45},
    };

    // std::ranges::transform (C++20) takes the whole input range and an optional
    // PROJECTION. The projection picks the field; the function maps it. Here each
    // user's age is projected out, then turned into an over-30 flag.
    std::vector<bool> over30;
    std::ranges::transform(users, std::back_inserter(over30),
        [](int a){ return a > 30; }, &User::age);
    std::cout << std::boolalpha << "over 30: ";
    for (bool flag : over30) std::cout << flag << ' ';
    std::cout << '\n';

    // views::transform is the LAZY counterpart: it maps on demand inside a pipeline
    // with no output container. Nothing is computed or stored until you iterate --
    // the algorithm writes eagerly, the view maps element by element as consumed.
    auto names = users | std::views::transform(&User::name);
    std::cout << "names (via view): ";
    for (const auto& n : names) std::cout << n << ' ';
    std::cout << '\n';
    return 0;
}
