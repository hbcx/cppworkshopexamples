#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{4, 0, 7, 0, 9, 0};

    // BAD: remove_if compacts and returns the new logical end, but on its own it
    // cannot resize the container -- an algorithm sees iterators, never the
    // container. Without the follow-up erase the size does not change at all, even
    // though the returned iterator already marks where the kept elements end.
    std::vector<int> bad = v;
    auto new_end = std::remove_if(bad.begin(), bad.end(),
                                  [](int n) { return n == 0; });
    std::cout << "kept elements (distance to new end): "
              << std::distance(bad.begin(), new_end) << '\n';           // 3
    std::cout << "container size, still unchanged:     " << bad.size() << '\n';  // 6

    // GOOD: pass that returned iterator to erase to actually drop the tail
    // (or use erase_if in C++20).
    std::vector<int> good = v;
    good.erase(std::remove_if(good.begin(), good.end(),
                              [](int n) { return n == 0; }),
               good.end());
    std::cout << "size after erase-remove:             " << good.size() << '\n';  // 3
    std::cout << "kept: ";
    for (int x : good) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
