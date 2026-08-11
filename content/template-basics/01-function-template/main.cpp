#include <iostream>
#include <string>

// A function template: one definition that works for many types. The compiler
// DEDUCES T from the arguments, so you call it like an ordinary function.
template <class T>
T maxOf(T a, T b) {
    return a > b ? a : b;
}

int main() {
    std::cout << "maxOf(3, 7)      = " << maxOf(3, 7) << "\n";        // T = int -> 7
    std::cout << "maxOf(2.5, 1.5)  = " << maxOf(2.5, 1.5) << "\n";    // T = double -> 2.5
    std::cout << "maxOf<int>(9, 4) = " << maxOf<int>(9, 4) << "\n";   // explicit T = int

    std::string a = "apple", b = "pear";
    std::cout << "maxOf(strings)   = " << maxOf(a, b) << "\n";        // T = std::string -> pear
    return 0;
}
