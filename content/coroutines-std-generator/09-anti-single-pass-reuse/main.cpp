#include <generator>
#include <iostream>
#include <vector>

std::generator<int> nums() {
    for (int i = 1; i <= 3; ++i)
        co_yield i;
}

// ANTI-PATTERN (described, NOT run -- it is undefined behaviour):
//
//   std::generator<int> g = nums();
//   for (int x : g) { ... }     // consumes the single-pass generator
//   for (int x : g) { ... }     // begin() again on an exhausted generator -> UB
//
// A std::generator is a single-pass input range: iterating it advances the one
// underlying coroutine, and there is no way to rewind it. Calling begin() a second
// time is undefined -- it is not a container you can walk again.

int main() {
    // FIX (a): if you just need to traverse more than once, call the factory again
    // to get a fresh generator each time.
    std::cout << "pass 1:";
    for (int x : nums())
        std::cout << " " << x;
    std::cout << "\n";
    std::cout << "pass 2:";
    for (int x : nums())
        std::cout << " " << x;
    std::cout << "\n";

    // FIX (b): if the values are needed repeatedly, materialize them once into a
    // container, which is a multi-pass range you can walk any number of times.
    std::vector<int> saved;
    for (int x : nums())
        saved.push_back(x);

    std::cout << "from a saved vector, twice:";
    for (int x : saved) std::cout << " " << x;
    for (int x : saved) std::cout << " " << x;
    std::cout << "\n";
    return 0;
}
