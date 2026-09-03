#include <iostream>
#include <iterator>
#include <vector>

int main() {
    // A reverse_iterator rit and its base() are offset by ONE: rit refers to
    // *(rit.base() - 1). Forgetting that is the classic reverse-iterator bug,
    // most often when converting a reverse_iterator back to a normal one to erase.

    std::vector<int> v{1, 2, 3, 4, 5};

    // Locate the element 3 with a reverse scan.
    auto rit = v.rbegin();
    while (rit != v.rend() && *rit != 3) ++rit;
    std::cout << "*rit = " << *rit << "\n";   // 3

    // ANTI-PATTERN: erasing at rit.base() removes the WRONG element -- base()
    // points one PAST rit's element (at 4), so v.erase(rit.base()) would erase 4.
    std::cout << "*(rit.base()) = " << *rit.base() << " (one past rit)\n";   // 4

    // FIX: to erase the element rit refers to, erase at rit.base() - 1. (Vector
    // erase takes a forward iterator, so this is the correct conversion.)
    v.erase(rit.base() - 1);
    std::cout << "after erasing rit's element:";
    for (int x : v) std::cout << " " << x;
    std::cout << "\n";   // 1 2 4 5
    return 0;
}
