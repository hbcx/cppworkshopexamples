#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> m;

    // insert returns pair<iterator, bool>; the bool is true when a NEW element
    // was added.
    auto r1 = m.insert({"alice", 1});
    std::cout << "insert alice:       inserted=" << (r1.second ? "yes" : "no")
              << " value=" << r1.first->second << '\n';

    // insert does NOT overwrite an existing key: it reports failure and leaves
    // the old value in place.
    auto r2 = m.insert({"alice", 999});
    std::cout << "insert alice again: inserted=" << (r2.second ? "yes" : "no")
              << " value=" << r2.first->second << '\n';   // still 1

    // emplace constructs the element in place; same unique-key semantics.
    auto r3 = m.emplace("bob", 2);
    std::cout << "emplace bob:        inserted=" << (r3.second ? "yes" : "no") << '\n';

    // To overwrite, assign through operator[] (or insert_or_assign in C++17).
    m["alice"] = 999;
    std::cout << "after m[alice]=999: value=" << m["alice"] << '\n';
    return 0;
}
