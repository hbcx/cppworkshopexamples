#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    std::vector<int> data{1, 0, 0, 2, 0, 3};

    // BAD: erasing by index while always advancing i. After erase(begin() + i)
    // every later element shifts down by one, but i still moves forward -- so the
    // element that slid into position i is never checked and survives. With two
    // zeros in a row, the second zero is skipped.
    std::vector<int> bad = data;
    for (std::size_t i = 0; i < bad.size(); ++i) {
        if (bad[i] == 0) {
            bad.erase(bad.begin() + static_cast<std::ptrdiff_t>(i));
        }
    }
    std::cout << "buggy index loop left: ";
    for (int x : bad) std::cout << x << ' ';
    std::cout << '\n';

    // (An iterator loop of the same shape -- erase(it) then ++it -- is worse:
    // erase invalidates it, so the following ++it is undefined behavior, not
    // just a skipped element.)

    // GOOD: the erase-remove idiom deletes every match in one pass, with no
    // index or iterator bookkeeping to get wrong.
    std::vector<int> good = data;
    good.erase(std::remove(good.begin(), good.end(), 0), good.end());
    std::cout << "erase-remove left:     ";
    for (int x : good) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
