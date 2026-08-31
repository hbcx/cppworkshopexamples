// Anti-pattern: iterating a flat_map with `auto&`.
//
// A flat_map keeps keys and values in separate vectors, so the iterator returns
// a temporary proxy pair<const K&, V&>, not a real element reference. A non-const
// auto& cannot bind that temporary, so the loop does not compile. Use `auto` or
// `const auto&`.

#include <flat_map>
#include <iostream>
#include <string>

int main() {
    std::flat_map<std::string, int> scores{{"ann", 3}, {"bob", 5}};

    // BAD (does not compile -- described):
    //   for (auto& [name, score] : scores) ...   // cannot bind the proxy temporary

    // GOOD 1: auto copies the proxy; its members are still references into the map,
    // so writing through them updates the map.
    for (auto [name, score] : scores) score += 0;   // (no-op read here)

    // GOOD 2: const auto& binds the temporary and lifetime-extends it.
    std::cout << "scores:\n";
    for (const auto& [name, score] : scores)
        std::cout << "  " << name << " -> " << score << '\n';

    // To actually modify a value, take the proxy by auto and assign through it.
    for (auto [name, score] : scores) score = score * 10;
    std::cout << "after x10: " << scores.at("ann") << ' ' << scores.at("bob") << '\n';
}
