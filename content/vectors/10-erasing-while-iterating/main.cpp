#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

static void print(const std::vector<int>& v, const char* label) {
    std::cout << label;
    for (int x : v) {
        std::cout << ' ' << x;
    }
    std::cout << "   (size=" << v.size() << ")\n";
}

int main() {
    // Goal in every block: remove every even number.

    // Anti-pattern: erase by index inside a size()-based loop. Erasing shifts the
    // later elements one step left, but ++i then steps PAST the element that moved
    // into the freed slot -- so adjacent matches are skipped. (It is also O(n^2):
    // each erase shifts the tail.)
    {
        std::vector<int> v{2, 2, 4, 3, 6, 7};
        for (std::size_t i = 0; i < v.size(); ++i) {
            if (v[i] % 2 == 0) {
                v.erase(v.begin() + static_cast<std::ptrdiff_t>(i));  // skips the next element
            }
        }
        print(v, "buggy erase-in-loop:");   // some evens survive
    }

    // Fix A: the erase-remove idiom -- one linear pass, no index bookkeeping.
    {
        std::vector<int> v{2, 2, 4, 3, 6, 7};
        v.erase(std::remove_if(v.begin(), v.end(),
                               [](int x) { return x % 2 == 0; }),
                v.end());
        print(v, "fix erase-remove:   ");
    }

    // Fix B: if you must loop, let erase return the next valid iterator and only
    // advance when you did NOT erase.
    {
        std::vector<int> v{2, 2, 4, 3, 6, 7};
        for (auto it = v.begin(); it != v.end(); ) {
            if (*it % 2 == 0) {
                it = v.erase(it);
            } else {
                ++it;
            }
        }
        print(v, "fix erase-return:   ");
    }
    return 0;
}
