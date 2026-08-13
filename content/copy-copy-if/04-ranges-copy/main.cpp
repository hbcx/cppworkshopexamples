#include <algorithm>
#include <vector>
#include <string>
#include <iterator>
#include <ranges>
#include <iostream>

struct User {
    std::string name;
    bool active;
};

int main() {
    std::vector<User> users{
        {"Ada", true}, {"Linus", false}, {"Grace", true},
    };

    // std::ranges::copy_if (C++20) takes the whole range, and a PROJECTION lets the
    // predicate test a member. Here copy the active users -- the predicate sees the
    // projected active flag; whole User elements are copied.
    std::vector<User> activeUsers;
    std::ranges::copy_if(users, std::back_inserter(activeUsers),
        [](bool a){ return a; }, &User::active);
    std::cout << "active users: ";
    for (const auto& u : activeUsers) std::cout << u.name << ' ';
    std::cout << '\n';   // Ada Grace

    // ranges::copy works straight from a VIEW: copy a transform pipeline into a
    // container in one call, with no begin()/end() and no intermediate.
    std::vector<std::string> names;
    std::ranges::copy(users | std::views::transform(&User::name),
                      std::back_inserter(names));
    std::cout << "all names:    ";
    for (const auto& n : names) std::cout << n << ' ';
    std::cout << '\n';   // Ada Linus Grace
    return 0;
}
