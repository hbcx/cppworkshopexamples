#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

int main() {
    std::vector<int> deck{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // ANTI-PATTERN: std::random_shuffle(v.begin(), v.end());
    //
    // The old two-argument std::random_shuffle was DEPRECATED in C++14 and
    // REMOVED in C++17 -- code using it stops compiling on a modern standard.
    // Even where it still exists it is a poor choice: its randomness came from
    // an unspecified source (typically std::rand), so you could not seed it in
    // a controlled, reproducible way, and rand-based shuffles are often biased.
    // We do not call it here; it may not exist at all under -std=c++17.

    // FIX: std::shuffle with an explicit engine. You control the seed, so the
    // result is reproducible, and the permutation is uniform (unbiased).
    std::mt19937 engine(9001);
    std::shuffle(deck.begin(), deck.end(), engine);

    std::cout << "shuffled with std::shuffle:";
    for (int x : deck) std::cout << ' ' << x;
    std::cout << '\n';

    // Same seed -> same order, which random_shuffle could never promise.
    std::vector<int> again{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::mt19937 same(9001);
    std::shuffle(again.begin(), again.end(), same);
    std::cout << "reproducible? " << std::boolalpha << (deck == again) << '\n';
    return 0;
}
