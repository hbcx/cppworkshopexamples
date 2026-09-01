#include <random>
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

int main() {
    std::mt19937 gen(5);

    // std::shuffle reorders a range with the engine -- the correct, unbiased
    // shuffle (Fisher-Yates internally). Never hand-roll one with rand() % n.
    std::vector<int> deck{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::shuffle(deck.begin(), deck.end(), gen);
    std::cout << "shuffled:";
    for (int x : deck) std::cout << ' ' << x;
    std::cout << "\n";

    // std::sample (C++17) draws k distinct items, writing them out through an
    // output iterator -- here into a fresh vector via back_inserter.
    std::vector<int> out;
    std::sample(deck.begin(), deck.end(), std::back_inserter(out), 3, gen);
    std::cout << "sample of 3:";
    for (int x : out) std::cout << ' ' << x;
    std::cout << "\n";
}
