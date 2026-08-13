#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    // BAD: a plain while loop that calls next_permutation in the CONDITION before
    // the body runs. next_permutation advances FIRST, so the loop never processes
    // the starting (sorted) permutation -- it is skipped, giving n! - 1.
    std::vector<int> bad{1, 2, 3};
    int badCount = 0;
    while (std::next_permutation(bad.begin(), bad.end())) {
        ++badCount;   // the initial 1 2 3 is never counted
    }
    std::cout << "while loop counted: " << badCount << " (missed the first)\n";

    // GOOD: a do-while processes the current permutation BEFORE stepping, so the
    // sorted starting arrangement is included and you get the full n!.
    std::vector<int> good{1, 2, 3};
    int goodCount = 0;
    do {
        ++goodCount;
    } while (std::next_permutation(good.begin(), good.end()));
    std::cout << "do-while counted:   " << goodCount << " (all of them)\n";
    return 0;
}
