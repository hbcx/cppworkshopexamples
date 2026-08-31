// std::flat_set (C++23) holds unique keys sorted in a vector. It offers the
// std::set interface -- contains, insert with dedup, ordered iteration -- over
// packed, contiguous storage.

#include <flat_set>
#include <iostream>

int main() {
    std::flat_set<int> ids{5, 1, 3, 1, 4};   // sorted and de-duplicated on build

    std::cout << "after build (sorted, unique):";
    for (int x : ids) std::cout << ' ' << x;   // 1 3 4 5
    std::cout << '\n';

    ids.insert(2);                             // stays sorted
    auto [it, added] = ids.insert(3);          // 3 already present
    std::cout << "insert(2) then insert(3) added? " << std::boolalpha << added << '\n';

    std::cout << "contains(4)? " << ids.contains(4) << '\n';
    std::cout << "final:";
    for (int x : ids) std::cout << ' ' << x;   // 1 2 3 4 5
    std::cout << '\n';
}
