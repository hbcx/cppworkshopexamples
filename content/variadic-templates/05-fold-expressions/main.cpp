#include <iostream>

// A fold expression applies a binary operator across a whole pack in ONE
// expression -- no recursion, no base case.
template <class... Ts>
auto sum(Ts... args) {
    return (args + ...);          // unary right fold: a + (b + (c + ...))
}

template <class... Ts>
bool allTrue(Ts... args) {
    return (args && ...);         // unary right fold over &&
}

template <class... Ts>
void printLine(const Ts&... args) {
    (std::cout << ... << args) << "\n";   // binary left fold: cout << a << b << ...
}

int main() {
    std::cout << "sum(1,2,3,4) = " << sum(1, 2, 3, 4) << "\n";   // 10
    std::cout << std::boolalpha;
    std::cout << "allTrue(true,true,false) = " << allTrue(true, true, false) << "\n";  // false
    printLine("x=", 42, " y=", 3.5);                             // x=42 y=3.5
    return 0;
}
