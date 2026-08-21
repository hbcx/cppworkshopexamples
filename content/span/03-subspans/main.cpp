#include <iostream>
#include <span>
#include <string_view>
#include <vector>

void print(std::string_view label, std::span<const int> s) {
    std::cout << label;
    for (int x : s) std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7};
    std::span<int> s(v);

    // Subviews are O(1) and copy nothing -- each is a new pointer+size into the
    // same data.
    print("first(3):     ", s.first(3));        // 0 1 2
    print("last(3):      ", s.last(3));         // 5 6 7
    print("subspan(2,4): ", s.subspan(2, 4));   // 2 3 4 5
    print("subspan(5):   ", s.subspan(5));      // 5 6 7 (offset to the end)

    // Subviews compose: a window of a window.
    print("chained:      ", s.subspan(1).first(3)); // 1 2 3
    return 0;
}
