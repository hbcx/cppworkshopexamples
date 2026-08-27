#include <iostream>
#include <string>
#include <utility>

// A structured binding declares several names at once from one object. For a
// std::pair (or std::tuple) it gives each element its own name, so you stop
// writing .first and .second.

std::pair<std::string, int> findUser() {
    return {"alice", 42};
}

int main() {
    // auto [a, b] introduces two names bound to the pair's elements.
    auto [name, id] = findUser();
    std::cout << "name = " << name << ", id = " << id << "\n";

    // It works on any std::pair, including one you build directly.
    std::pair<int, int> point{3, 4};
    auto [x, y] = point;
    std::cout << "point = (" << x << ", " << y << ")\n";

    // With plain auto the names are copies of the elements: you can read and
    // write them without touching the source.
    x += 10;
    std::cout << "x after += 10: " << x
              << ", point.first still " << point.first << "\n";
    return 0;
}
