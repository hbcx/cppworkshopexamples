#include <iostream>
#include <vector>
#include <functional>

int main() {
    std::vector<std::function<int()>> byRef, byValue;

    // BAD: capture a shared variable by REFERENCE and store the closures. They all
    // refer to the SAME i, so after the loop every closure returns its FINAL value,
    // not 0, 1, 2. (i is declared outside the loop so the references stay valid;
    // capturing a loop-LOCAL by reference and using it later would dangle instead.)
    int i = 0;
    for (i = 0; i < 3; ++i)
        byRef.push_back([&i] { return i; });

    // GOOD: capture by VALUE, so each closure snapshots the counter at its own
    // iteration.
    for (int j = 0; j < 3; ++j)
        byValue.push_back([j] { return j; });

    std::cout << "by reference (all see final i = " << i << "): ";
    for (auto& f : byRef) std::cout << f() << ' ';
    std::cout << '\n';

    std::cout << "by value (each its own snapshot): ";
    for (auto& f : byValue) std::cout << f() << ' ';
    std::cout << '\n';
    return 0;
}
