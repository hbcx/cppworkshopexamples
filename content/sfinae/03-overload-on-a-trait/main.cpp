#include <iostream>
#include <string>
#include <type_traits>

// Before if constexpr (C++17) and concepts (C++20), the way to run different
// code for different kinds of type was a pair of overloads, each enabled by a
// trait through enable_if. The conditions must be mutually EXCLUSIVE, or a call
// that satisfies both is ambiguous -- see the overlapping-conditions anti-pattern.

template <class T>
typename std::enable_if<std::is_integral<T>::value, std::string>::type
classify(T) { return "integer"; }

template <class T>
typename std::enable_if<std::is_floating_point<T>::value, std::string>::type
classify(T) { return "floating point"; }

int main() {
    std::cout << "classify(42)   -> " << classify(42) << "\n";
    std::cout << "classify(3.14) -> " << classify(3.14) << "\n";
    std::cout << "classify(2.0f) -> " << classify(2.0f) << "\n";
    return 0;
}
