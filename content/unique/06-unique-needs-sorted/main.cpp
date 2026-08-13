#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{3, 1, 3, 2, 1, 3, 2};

    // BAD: calling unique on UNSORTED data expecting all duplicates gone. unique
    // only removes CONSECUTIVE equal elements, and here no two equal values are
    // adjacent, so nothing at all is removed -- the "deduped" vector still has
    // every duplicate.
    std::vector<int> bad = v;
    bad.erase(std::unique(bad.begin(), bad.end()), bad.end());
    std::cout << "unique on unsorted: ";
    for (int x : bad) std::cout << x << ' ';
    std::cout << '\n';

    // GOOD: sort first so equal values are neighbours, then unique removes them
    // all -- the sort + unique + erase idiom.
    std::vector<int> good = v;
    std::sort(good.begin(), good.end());
    good.erase(std::unique(good.begin(), good.end()), good.end());
    std::cout << "sort then unique:   ";
    for (int x : good) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
