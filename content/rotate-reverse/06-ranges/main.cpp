#include <algorithm>
#include <iterator>
#include <ranges>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6};

    // ranges::reverse takes the whole range directly, no begin/end pair.
    std::ranges::reverse(v);
    std::cout << "ranges::reverse: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // ranges::rotate takes the range and the new-first iterator, and returns a
    // SUBRANGE whose begin is the new position of the old first element.
    auto r = std::ranges::rotate(v, v.begin() + 2);
    std::cout << "ranges::rotate:  ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    std::cout << "old first now at index "
              << std::distance(v.begin(), r.begin()) << '\n';
    return 0;
}
