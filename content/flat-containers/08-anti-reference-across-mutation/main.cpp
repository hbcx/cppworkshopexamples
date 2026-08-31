// Anti-pattern: keeping a reference or iterator into a flat container across an
// insert or erase.
//
// A flat container is a vector underneath, so a mutation can shift or reallocate
// storage and invalidate every reference and iterator -- unlike std::map, where
// nodes never move. Look the key up again after the change.

#include <flat_map>
#include <iostream>
#include <string>

int main() {
    std::flat_map<int, std::string> m{{1, "a"}, {3, "c"}};

    std::string& value = m.at(1);      // reference into the flat storage
    std::cout << "before insert, value for key 1: " << value << '\n';  // fine so far

    // BAD (undefined -- described, not run):
    //   m.emplace(2, "b");            // insert shifts the vector
    //   std::cout << value;           // `value` may now dangle

    // GOOD: do the insert, then look the key up again for a fresh reference.
    m.emplace(2, "b");
    std::string& fresh = m.at(1);      // valid: fetched after the mutation
    std::cout << "value for key 1 after insert: " << fresh << '\n';

    std::cout << "contents:";
    for (auto [k, v] : m) std::cout << ' ' << k << '=' << v;
    std::cout << '\n';
}
