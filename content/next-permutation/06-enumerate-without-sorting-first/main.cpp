#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    // BAD: enumerating from an ARBITRARY starting order. next_permutation only
    // steps to lexicographically GREATER arrangements and stops when it wraps, so
    // starting at 2 1 3 visits only the permutations that come after it -- missing
    // 1 2 3 and 1 3 2 entirely.
    std::vector<int> bad{2, 1, 3};
    int badCount = 0;
    do { ++badCount; } while (std::next_permutation(bad.begin(), bad.end()));
    std::cout << "starting unsorted (2 1 3): " << badCount << " permutations\n";

    // GOOD: sort first to begin at the smallest permutation, then the loop sees all
    // n! of them.
    std::vector<int> good{2, 1, 3};
    std::sort(good.begin(), good.end());
    int goodCount = 0;
    do { ++goodCount; } while (std::next_permutation(good.begin(), good.end()));
    std::cout << "starting sorted (1 2 3):   " << goodCount << " permutations\n";
    return 0;
}
