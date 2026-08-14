#include <algorithm>
#include <iostream>
#include <vector>

static void printAll(const char* label, const std::vector<int>& v) {
    std::cout << label;
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    // A round-robin: after serving player 1, move them to the BACK so everyone
    // gets a turn -- the order should cycle to 2 3 4 1, losing nobody.

    // ANTI-PATTERN: reach for shift_left to "advance" the queue. shift_left drops
    // the element that falls off the front: player 1 is simply gone, and a slot
    // at the back is left moved-from. The queue silently shrinks.
    std::vector<int> shifted{1, 2, 3, 4};
    auto end = std::shift_left(shifted.begin(), shifted.end(), 1);
    std::cout << "shift_left result (valid part):";
    for (auto it = shifted.begin(); it != end; ++it) std::cout << ' ' << *it;
    std::cout << "   <- player 1 was lost\n";

    // FIX: std::rotate WRAPS the displaced element around to the back instead of
    // discarding it, so every player stays in the cycle.
    std::vector<int> rotated{1, 2, 3, 4};
    std::rotate(rotated.begin(), rotated.begin() + 1, rotated.end());
    printAll("rotate result:                ", rotated);   // 2 3 4 1, nobody lost

    // Rule: shift DROPS what falls off the end; rotate WRAPS it. Use shift for a
    // sliding window that should forget old data, rotate to cycle without loss.
    return 0;
}
