#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

// Print a range on one line.
static void print(const char* label, const std::vector<int>& v) {
    std::cout << label;
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    std::vector<int> deck{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // std::shuffle needs a random number ENGINE, not just a range. The engine
    // is the source of randomness; shuffle asks it for numbers and uses them to
    // build a uniformly random permutation -- every ordering equally likely.
    // We seed with a fixed number here so the run is reproducible (same seed ->
    // same shuffle). For real randomness, seed from std::random_device instead.
    std::mt19937 engine(12345);
    std::shuffle(deck.begin(), deck.end(), engine);
    print("shuffled:      ", deck);

    // The same seed reproduces the same order -- useful for tests and for
    // replaying a game from a saved seed.
    std::vector<int> again{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::mt19937 same(12345);
    std::shuffle(again.begin(), again.end(), same);
    print("same seed:     ", again);
    std::cout << "orders match?  " << std::boolalpha << (deck == again) << '\n';

    // Shuffling again with the SAME engine keeps advancing it, so the next
    // permutation differs -- one engine, a fresh draw each time.
    std::shuffle(deck.begin(), deck.end(), engine);
    print("shuffled again:", deck);
    return 0;
}
