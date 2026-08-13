#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{3, 6, 9, 12};

    // BAD: for_each with a lambda just to visit every element of the whole
    // container. It needs the begin/end pair and a lambda wrapper for what is
    // really a plain loop.
    std::for_each(v.begin(), v.end(), [](int x) {
        std::cout << "for_each:  " << x << '\n';
    });

    // GOOD: a range-based for loop reads the same intent with no iterators and no
    // lambda -- and it can use break and continue, which for_each cannot.
    for (int x : v) {
        std::cout << "range-for: " << x << '\n';
    }
    return 0;
}
