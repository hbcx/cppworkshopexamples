#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{5, 5, 5, 8, 8, 1};

    // BAD: unique compacts and returns the new logical end, but like remove it
    // cannot resize the container. Without the follow-up erase the size does not
    // change -- the returned iterator already marks the end of the unique run, but
    // nothing acts on it, and the tail past it holds unspecified leftovers.
    std::vector<int> bad = v;
    auto new_end = std::unique(bad.begin(), bad.end());
    std::cout << "unique run length (distance): "
              << std::distance(bad.begin(), new_end) << '\n';           // 3
    std::cout << "container size, unchanged:    " << bad.size() << '\n';  // 6

    // GOOD: pass the returned iterator to erase to drop the tail.
    std::vector<int> good = v;
    good.erase(std::unique(good.begin(), good.end()), good.end());
    std::cout << "size after unique+erase:      " << good.size() << '\n';  // 3
    std::cout << "kept: ";
    for (int x : good) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
