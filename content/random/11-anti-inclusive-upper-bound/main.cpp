#include <random>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> items{10, 20, 30, 40};   // valid indices are 0..3
    std::mt19937 gen(3);

    // TRAP: uniform_int_distribution(0, size) has the CLOSED range [0, size], so
    // it can return size itself -- one past the last valid index. items[size]
    // would be out of bounds. The upper bound is INCLUDED, unlike most C++
    // ranges. We prove it returns the bad index without ever dereferencing it:
    std::uniform_int_distribution<int> bad(0, static_cast<int>(items.size()));
    int max_seen = 0;
    for (int i = 0; i < 1000; ++i) max_seen = std::max(max_seen, bad(gen));
    std::cout << "bad dist max index over 1000 draws = " << max_seen
              << "  (size is " << items.size()
              << ", so index " << items.size() << " is out of range)\n";

    // FIX: the top valid index is size - 1. Give the distribution that.
    std::uniform_int_distribution<int> ok(0, static_cast<int>(items.size()) - 1);
    std::cout << "valid picks:";
    for (int i = 0; i < 5; ++i) std::cout << ' ' << items[ok(gen)];
    std::cout << "\n";
}
