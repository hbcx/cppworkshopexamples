#include <iostream>

int main() {
    // A by-value capture is read-only inside the lambda by default. `mutable` lets
    // the body change the closure's OWN copy -- and that copy persists between
    // calls, so the lambda carries state.
    int start = 0;
    auto counter = [start]() mutable { return ++start; };
    std::cout << counter() << ' ' << counter() << ' ' << counter() << '\n';   // 1 2 3

    // The changes stay INSIDE the closure; the captured variable outside is never
    // touched -- the closure has its own copy.
    std::cout << "start outside is still " << start << '\n';   // 0
    return 0;
}
