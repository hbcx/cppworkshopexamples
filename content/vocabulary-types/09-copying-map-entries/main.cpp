#include <iostream>
#include <map>
#include <string>
#include <utility>

// A key type that counts its own copy-construction, so the hidden copies made
// by a wrong range-for become visible.
struct CountedKey {
    std::string name;
    CountedKey(const char* n) : name(n) {}                       // build from a literal
    CountedKey(const CountedKey& o) : name(o.name) { ++copies; } // count copies
    bool operator<(const CountedKey& o) const { return name < o.name; }
    static int copies;
};
int CountedKey::copies = 0;

int main() {
    std::map<CountedKey, int> m;
    m.emplace("ada", 1);
    m.emplace("bea", 2);
    m.emplace("cyd", 3);

    // The element type is pair<const CountedKey, int>. Typing the loop variable
    // pair<CountedKey, int> (non-const key) does NOT match, so each iteration
    // copies the whole entry into the loop variable -- silently, every element.
    CountedKey::copies = 0;
    int sumBad = 0;
    for (std::pair<CountedKey, int> entry : m) {
        sumBad += entry.second;
    }
    std::cout << "[buggy]   copies during loop = " << CountedKey::copies << '\n';

    // Fix: bind by const reference (structured bindings do the same).
    CountedKey::copies = 0;
    int sumGood = 0;
    for (const auto& entry : m) {
        sumGood += entry.second;
    }
    std::cout << "[correct] copies during loop = " << CountedKey::copies << '\n';

    std::cout << "same sum? " << (sumBad == sumGood ? "yes" : "no") << '\n';
    return 0;
}
