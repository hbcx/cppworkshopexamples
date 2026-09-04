#include <iostream>
#include <iterator>
#include <list>
#include <vector>

int main() {
    std::vector<int> v{10, 20, 30, 40, 50};

    // std::next returns a NEW iterator n steps forward; it does not modify its
    // argument. The default step is 1.
    auto it = v.begin();
    std::cout << "*it          = " << *it << "\n";               // 10
    std::cout << "*next(it)    = " << *std::next(it) << "\n";    // 20
    std::cout << "*next(it, 3) = " << *std::next(it, 3) << "\n"; // 40
    std::cout << "*it still    = " << *it << "\n";               // 10 -- next did not move it

    // std::prev is the mirror: n steps backward (needs a bidirectional iterator).
    std::cout << "*prev(end)    = " << *std::prev(v.end()) << "\n";    // 50 (last element)
    std::cout << "*prev(end, 2) = " << *std::prev(v.end(), 2) << "\n"; // 40

    // Because they return a value, they compose in expressions -- e.g. a range
    // that skips the first and last element, on a list that has no it + n at all.
    std::list<int> lst{1, 2, 3, 4, 5};
    std::cout << "middle of list:";
    for (auto i = std::next(lst.begin()); i != std::prev(lst.end()); ++i)
        std::cout << " " << *i;
    std::cout << "\n";   // 2 3 4
    return 0;
}
