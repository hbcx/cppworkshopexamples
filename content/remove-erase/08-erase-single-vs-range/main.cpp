#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> data{2, 0, 0, 5, 0, 8};

    // BAD: the erase-remove idiom with the second argument to erase left off.
    // std::remove returns the new logical end; erase(it) with a SINGLE iterator
    // is a valid overload that deletes just ONE element at that position. So this
    // compiles, runs, and removes a single leftover instead of the whole tail --
    // the container barely shrinks.
    std::vector<int> bad = data;
    bad.erase(std::remove(bad.begin(), bad.end(), 0));   // missing bad.end()
    std::cout << "size after single-arg erase: " << bad.size() << '\n';  // 5, not 3

    // GOOD: pass the end iterator so erase(first, last) drops the whole moved-from
    // tail that remove left behind.
    std::vector<int> good = data;
    good.erase(std::remove(good.begin(), good.end(), 0), good.end());
    std::cout << "size after range erase:      " << good.size() << '\n';  // 3
    std::cout << "kept: ";
    for (int x : good) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
