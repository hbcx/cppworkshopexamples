#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{3, 1, 4, 1, 5, 1, 9};

    // std::remove does NOT erase. It shifts the elements you keep to the front,
    // in their original order, and returns an iterator to the new logical end.
    // The elements from there to v.end() are leftovers and must not be read.
    auto new_end = std::remove(v.begin(), v.end(), 1);

    std::cout << "size right after remove: " << v.size() << '\n';  // still 7

    // The container is only actually shortened when erase drops the tail
    // [new_end, end). This two-step pair is the erase-remove idiom.
    v.erase(new_end, v.end());

    std::cout << "size after erase:        " << v.size() << '\n';   // 4
    std::cout << "kept: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
