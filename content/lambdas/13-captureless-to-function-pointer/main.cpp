#include <iostream>

// A C-style API that wants a plain function pointer (think qsort or a callback
// registration in a C library).
static int applyCallback(int (*fn)(int), int x) { return fn(x); }

int main() {
    // A lambda with NO captures converts implicitly to a plain function pointer,
    // because it holds no state -- so it can be handed to APIs that take one.
    int (*fp)(int) = [](int x) { return x * x; };
    std::cout << "through a function pointer: " << fp(6) << '\n';   // 36

    std::cout << "passed to a C-style API: "
              << applyCallback([](int x) { return x + 100; }, 5) << '\n';   // 105

    // A CAPTURING lambda holds state a bare pointer cannot carry, so it does NOT
    // convert. This would not compile:
    //   int factor = 3;
    //   int (*bad)(int) = [factor](int x){ return x * factor; };
    return 0;
}
