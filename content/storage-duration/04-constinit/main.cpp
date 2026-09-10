#include <iostream>

// constinit forces STATIC (compile-time) initialization: no dynamic initializer
// runs at startup, so the value is ready before any other static could read it.
// The initializer must therefore be a constant expression.
constinit int g_limit = 10 * 8;

constexpr int square(int x) { return x * x; }
constinit int g_squared = square(9);   // still a constant initializer

int main() {
    std::cout << "g_limit   = " << g_limit << "\n";
    std::cout << "g_squared = " << g_squared << "\n";

    // constinit controls initialization, not mutability -- unlike constexpr, the
    // variable is a normal mutable object at runtime.
    g_limit += 1;
    std::cout << "g_limit after += 1 = " << g_limit << "\n";
    return 0;
}
