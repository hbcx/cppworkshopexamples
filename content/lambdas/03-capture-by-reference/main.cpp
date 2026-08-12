#include <iostream>

int main() {
    int total = 0;

    // [&total] captures a REFERENCE, so the closure works on the ORIGINAL variable,
    // not a copy. Each call changes the real total.
    auto add = [&total](int x) { total += x; };
    add(5);
    add(10);
    add(3);
    std::cout << "total after adds: " << total << '\n';   // 18

    // Because it is the same variable, a change made outside is seen by the closure
    // too -- the reference always reads the current value.
    total = 100;
    add(1);
    std::cout << "total: " << total << '\n';   // 101
    return 0;
}
