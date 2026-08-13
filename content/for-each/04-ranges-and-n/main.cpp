#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

struct User {
    std::string name;
    int visits;
};

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7};

    // for_each_n (C++17) applies the callable to the first N elements, taking a
    // START iterator and a COUNT instead of an end iterator.
    std::for_each_n(v.begin(), 3,
                    [](int x) { std::cout << "first-n: " << x << '\n'; });

    // ranges::for_each takes the whole range and an optional PROJECTION, so the
    // callable receives a chosen field rather than the whole element.
    std::vector<User> users{{"Ada", 5}, {"Ben", 2}, {"Cid", 9}};
    std::ranges::for_each(users,
                          [](int n) { std::cout << "visits: " << n << '\n'; },
                          &User::visits);
    return 0;
}
