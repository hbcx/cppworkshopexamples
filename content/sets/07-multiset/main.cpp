#include <iostream>
#include <set>

int main() {
    std::multiset<int> m{1, 2, 2, 2, 3, 3};   // duplicates are kept, not collapsed

    std::cout << "size = " << m.size() << " (duplicates retained)\n";
    std::cout << "count(2) = " << m.count(2) << '\n';

    // equal_range returns [first, last) covering every element equal to the key.
    auto [lo, hi] = m.equal_range(3);   // structured bindings (C++17)
    std::cout << "the 3s:";
    for (auto it = lo; it != hi; ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << '\n';

    // erase(key) removes ALL matches; erase(iterator) removes just one.
    m.erase(m.find(2));   // drop a single 2
    std::cout << "after erasing one 2, count(2) = " << m.count(2) << '\n';
    return 0;
}
