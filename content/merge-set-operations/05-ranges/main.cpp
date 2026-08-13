#include <algorithm>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>
#include <iostream>

struct User {
    std::string name;
    int id;
};

int main() {
    // Both sorted by id.
    std::vector<User> team{{"Ada", 1}, {"Ben", 3}, {"Cid", 5}};
    std::vector<User> oncall{{"Ben", 3}, {"Dan", 4}, {"Cid", 5}};

    // ranges::set_intersection takes both ranges, a comparator ({} = default), and
    // a PROJECTION PER RANGE, so the sets are compared by id while whole User
    // objects are written to the output.
    std::vector<User> both;
    std::ranges::set_intersection(team, oncall, std::back_inserter(both),
                                  {}, &User::id, &User::id);

    std::cout << "on the team AND on call: ";
    for (const auto& u : both) std::cout << u.name << ' ';
    std::cout << '\n';
    return 0;
}
