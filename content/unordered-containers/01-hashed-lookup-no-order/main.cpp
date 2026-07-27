// std::unordered_map maps a key to a value like std::map does, but it is a hash
// table, not a tree: the key is hashed straight to a bucket, so lookup is O(1)
// on average instead of O(log n). The price is that there is no order at all --
// iterating does not give you insertion order or sorted order, only "some order"
// that depends on the hashes and the bucket layout.

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::unordered_map<std::string, int> ages;
    ages["alice"] = 30;
    ages["bob"] = 25;
    ages["carol"] = 41;
    ages.insert(std::make_pair(std::string("dave"), 19));

    // Lookup is by key. find returns an iterator (end() if absent); at throws on
    // a missing key; count is 0 or 1 because keys are unique.
    std::cout << "bob   -> " << ages.find("bob")->second << "\n";
    std::cout << "carol -> " << ages.at("carol") << "\n";
    std::cout << "has eve? " << (ages.count("eve") ? "yes" : "no") << "\n";
    std::cout << "size = " << ages.size() << "\n";

    // Iterating visits every element, but in an UNSPECIFIED order: not insertion
    // order, not key order, and it can differ between library implementations. So
    // we copy the keys out and sort them ourselves before printing -- the only
    // safe way to produce stable output from a hash container.
    std::vector<std::string> keys;
    for (std::unordered_map<std::string, int>::const_iterator it = ages.begin();
         it != ages.end(); ++it) {
        keys.push_back(it->first);
    }
    std::sort(keys.begin(), keys.end());

    std::cout << "keys (sorted for display):";
    for (std::size_t i = 0; i < keys.size(); ++i) {
        std::cout << " " << keys[i];
    }
    std::cout << "\n";
    return 0;
}
