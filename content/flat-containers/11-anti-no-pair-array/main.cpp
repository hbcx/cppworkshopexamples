// Anti-pattern: treating a flat_map as one contiguous array of pairs.
//
// flat_map is struct-of-arrays: a key vector and a parallel value vector. There
// is no pair<K, V>[] to point at and no data() returning one. The two contiguous
// arrays are keys() and values(), with keys()[i] paired to values()[i].

#include <flat_map>
#include <iostream>
#include <string>

int main() {
    std::flat_map<int, std::string> m{{200, "OK"}, {404, "Not Found"}, {500, "Error"}};

    // BAD (wrong -- described, not run):
    //   auto* p = &(*m.begin());   // this is a proxy pair, NOT an element in an array
    //   walk p[0], p[1], ...       // there is no contiguous pair array to walk
    //   m.data();                  // no such member either

    // GOOD: keys() and values() are the two contiguous arrays.
    const auto& keys = m.keys();
    const auto& vals = m.values();
    std::cout << "keys() is contiguous, size " << keys.size()
              << ", first key " << keys.front() << '\n';
    std::cout << "paired columns:\n";
    for (std::size_t i = 0; i < keys.size(); ++i)
        std::cout << "  " << keys[i] << " -> " << vals[i] << '\n';
}
