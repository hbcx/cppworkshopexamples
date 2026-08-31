// flat_map and std::map share the sorted interface but trade opposite ways:
//   std::map   -- node tree: O(log n) insert, references STAY valid, pointer-chasing
//   flat_map   -- sorted vector: O(n) insert, references invalidated, cache-friendly
// Reach for flat_map when you build once and read many times.

#include <flat_map>
#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<int, std::string> tree{{1, "a"}, {2, "b"}};
    std::flat_map<int, std::string> flat{{1, "a"}, {2, "b"}};

    // Same sorted contents and interface.
    std::cout << "both iterate in key order:\n";
    for (auto [k, v] : flat) std::cout << "  flat " << k << " -> " << v << '\n';

    // std::map keeps references valid across an insert (node-based).
    std::string& ref = tree.at(1);
    tree.emplace(3, "c");                  // insert -- does not move existing nodes
    std::cout << "map reference after insert: " << ref << " (still valid)\n";

    // flat_map would invalidate that reference: an insert shifts the vector,
    // just like std::vector. So do not hold references into a flat_map across
    // an insert or erase -- look the key up again instead.
    flat.emplace(3, "c");
    std::cout << "flat_map re-fetched after insert: " << flat.at(1) << '\n';
}
