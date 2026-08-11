#include <iostream>

// A consteval function can only be called with arguments that are themselves
// constant expressions -- it always runs at compile time.
consteval int squared(int x) { return x * x; }

// BAD: an ordinary function that forwards a run-time parameter to a consteval
// call does NOT compile -- y is not a constant expression inside f:
//   int f(int y) { return squared(y); }   // COMPILE ERROR

// GOOD 1: keep the value constant -- call with a literal or a constexpr.
constexpr int kNine = squared(3);

// GOOD 2: make the caller consteval too, so the constant-ness propagates.
consteval int squaredPlusOne(int x) { return squared(x) + 1; }

int main() {
    std::cout << "squared(3)        = " << kNine              << "\n";   // 9
    std::cout << "squared(4)        = " << squared(4)         << "\n";   // 16, literal is fine
    std::cout << "squaredPlusOne(5) = " << squaredPlusOne(5)  << "\n";   // 26

    // int r = 6;
    // std::cout << squared(r);   // COMPILE ERROR: r is a run-time value, and a
    //                            // consteval call cannot run at run time. Fix:
    //                            // make r constexpr, or use a plain constexpr fn.
    std::cout << "done\n";
}
