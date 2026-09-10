#include <iostream>

int expensiveSeed() {
    std::cout << "  (computing the seed -- happens once)\n";
    return 100;
}

int nextId() {
    // A function-local static is initialized ONCE, the first time control passes
    // through this line, and then persists across every later call.
    static int counter = expensiveSeed();
    return ++counter;
}

int main() {
    std::cout << "ids:";
    for (int i = 0; i < 4; ++i)
        std::cout << " " << nextId();
    std::cout << "\n";
    return 0;
}
