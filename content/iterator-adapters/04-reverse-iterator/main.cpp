#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};

    // rbegin()/rend() return reverse iterators: ++ moves toward the FRONT, so an
    // ordinary loop walks the range backwards without touching the container.
    std::cout << "reversed:";
    for (auto it = v.rbegin(); it != v.rend(); ++it) std::cout << " " << *it;
    std::cout << "\n";   // 5 4 3 2 1

    // They are ordinary iterators, so algorithms work on them. Find the LAST even
    // number by searching from the back.
    auto rit = std::find_if(v.rbegin(), v.rend(), [](int x){ return x % 2 == 0; });
    std::cout << "last even = " << *rit << "\n";   // 4

    // A reverse_iterator wraps a base() iterator, offset by ONE: rit refers to
    // *(rit.base() - 1). So base() points at the element AFTER rit in forward order.
    std::cout << "*(rit.base())     = " << *rit.base() << "\n";        // 5
    std::cout << "*(rit.base() - 1) = " << *(rit.base() - 1) << "\n";  // 4 (rit's element)

    // std::make_reverse_iterator (C++14) builds one from a base iterator when you
    // do not have rbegin at hand.
    return 0;
}
