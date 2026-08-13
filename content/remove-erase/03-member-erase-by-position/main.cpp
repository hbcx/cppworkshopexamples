#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    std::vector<int> v{5, 6, 7, 8, 9, 10};

    // erase(it) removes the single element at a position and returns an iterator
    // to the element that followed it. Use the return value -- the old iterator
    // is invalid after the call.
    auto it = std::find(v.begin(), v.end(), 7);
    if (it != v.end()) {
        auto next = v.erase(it);           // removes 7; next points at 8
        std::cout << "after erasing 7, next is " << *next << '\n';
    }

    // erase(first, last) removes a whole range in one call. Drop the last two.
    v.erase(v.end() - 2, v.end());

    std::cout << "left: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
