#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

struct User {
    std::string name;
    int age;
    bool active;
};

int main() {
    std::vector<User> users{
        {"Ada", 36, true},
        {"Linus", 28, true},
        {"Grace", 45, false},
    };

    std::cout << std::boolalpha;

    // The std::ranges versions (C++20) take the whole range -- no begin()/end()
    // pair -- and an optional PROJECTION that picks what the predicate sees. Here
    // &User::age projects each user to its age before the predicate runs.
    bool allAdults = std::ranges::all_of(users, [](int a){ return a >= 18; }, &User::age);
    std::cout << "all adults?   " << allAdults << '\n';   // true

    // A projection can be a pointer to a data member or a method, or any callable.
    // &User::active projects to the bool member, so the predicate just negates it.
    bool anyInactive = std::ranges::any_of(users, [](bool a){ return !a; }, &User::active);
    std::cout << "any inactive? " << anyInactive << '\n';   // true

    bool noneMinor = std::ranges::none_of(users, [](int a){ return a < 18; }, &User::age);
    std::cout << "none minors?  " << noneMinor << '\n';   // true
    return 0;
}
