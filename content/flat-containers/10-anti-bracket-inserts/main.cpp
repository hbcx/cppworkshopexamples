// Anti-pattern: using operator[] to read a possibly-missing key.
//
// flat_map::operator[] inserts a default value when the key is absent, and that
// insert is O(n) (it shifts the vector). Reading through [] silently grows the
// map. Use contains / find / at to read without inserting.

#include <flat_map>
#include <iostream>
#include <string>

int main() {
    std::flat_map<std::string, int> counts{{"a", 1}, {"b", 2}};
    std::cout << "size before: " << counts.size() << '\n';

    // BAD: this "read" of a missing key inserts a default 0 -- and shifts the vector.
    int accidental = counts["zzz"];
    std::cout << "counts[\"zzz\"] returned " << accidental
              << ", size now " << counts.size() << " (inserted!)\n";

    // GOOD: check first, and only read when present.
    std::string key = "yyy";
    if (counts.contains(key))
        std::cout << key << " = " << counts.at(key) << '\n';
    else
        std::cout << key << " absent; not inserted, size still " << counts.size() << '\n';
}
