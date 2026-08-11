#include <iostream>

// A constexpr function can run at COMPILE TIME and at RUN TIME. Since C++14 it
// may use loops and local variables, so it reads like ordinary code.
constexpr int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

int main() {
    // Compile time: the result is a constant, checked by static_assert and
    // usable wherever a constant is required.
    static_assert(factorial(5) == 120, "factorial(5) must be 120");
    constexpr int f6 = factorial(6);        // computed by the compiler
    std::cout << "factorial(6) = " << f6 << "\n";

    // Run time: the SAME function, called with a value known only at run time.
    int n = 4;
    std::cout << "factorial(" << n << ") = " << factorial(n) << "\n";
    return 0;
}
