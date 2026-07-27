// Anti-pattern: treating the iteration order of an unordered_map as if it were
// stable. It is not. The order depends on the hashes and the current bucket
// count, so it is not insertion order, not sorted order, it shifts when unrelated
// keys are added, and it differs between standard library implementations.

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

int main() {
    std::unordered_map<std::string, int> config;
    config["width"] = 800;
    config["height"] = 600;
    config["depth"] = 24;
    config["fps"] = 60;

    // THE TRAP: serializing by iterating the map directly --
    //
    //   for (const auto& kv : config)
    //       out << kv.first << "=" << kv.second << "\n";
    //
    // -- writes the lines in an UNSPECIFIED order. A golden-file test that
    // compares this output against a fixed expected file is flaky for a reason
    // that looks like nothing; a config or manifest written this way produces a
    // different diff every time an unrelated key is added. We do not even print
    // that order here, because it would differ between g++ and clang++.

    // THE FIX (1): choose an order explicitly. Copy the entries out and sort.
    std::vector<std::pair<std::string, int> > entries(config.begin(), config.end());
    std::sort(entries.begin(), entries.end());
    std::cout << "sorted explicitly before output:\n";
    for (std::size_t i = 0; i < entries.size(); ++i) {
        std::cout << "  " << entries[i].first << " = " << entries[i].second << "\n";
    }

    // THE FIX (2): if you want that order for free at every step, std::map keeps
    // its keys sorted by construction. Reach for it when stable output is a
    // requirement, not an accident.
    std::map<std::string, int> ordered(config.begin(), config.end());
    std::cout << "same data through std::map (ordered by construction):\n";
    for (std::map<std::string, int>::const_iterator it = ordered.begin();
         it != ordered.end(); ++it) {
        std::cout << "  " << it->first << " = " << it->second << "\n";
    }
    return 0;
}
