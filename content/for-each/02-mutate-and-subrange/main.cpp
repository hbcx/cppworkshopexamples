#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6};

    // Take the element by REFERENCE to modify it in place. Here every element is
    // scaled by 10.
    std::for_each(v.begin(), v.end(), [](int& x) { x *= 10; });

    // for_each takes an iterator pair, so it applies naturally to a SUB-RANGE.
    // Add 1 to only the first three elements.
    std::for_each(v.begin(), v.begin() + 3, [](int& x) { x += 1; });

    std::cout << "result: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
