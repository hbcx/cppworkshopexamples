#include <cstddef>
#include <iostream>

// Pre-C++17, you process a pack by peeling off the FIRST argument and recursing
// on the REST. A non-variadic overload is the base case that ends the recursion.
void print() {                                   // base case: empty pack
    std::cout << "\n";
}
template <class First, class... Rest>
void print(First first, Rest... rest) {          // one argument + the remaining pack
    std::cout << first;
    if (sizeof...(rest) > 0) std::cout << ", ";
    print(rest...);                              // recurse on the shorter pack
}

int main() {
    print(1, 2.5, "three", 'x');   // 1, 2.5, three, x
    print(42);                     // 42
    return 0;
}
