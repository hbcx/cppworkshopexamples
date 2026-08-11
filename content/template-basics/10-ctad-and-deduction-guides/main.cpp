#include <iostream>
#include <string>
#include <vector>

// In C++17, class template argument deduction (CTAD) deduces the class arguments
// from the constructor -- Pair(1, 2.5) gives Pair<int, double>, no <...> needed.
template <class A, class B>
struct Pair {
    A first;
    B second;
    Pair(A a, B b) : first(a), second(b) {}
};

// A DEDUCTION GUIDE tells the compiler how to deduce the class arguments when the
// constructor alone is not enough -- here, decay a string literal to std::string.
template <class T>
struct Wrapper {
    T value;
    Wrapper(T v) : value(v) {}
};
Wrapper(const char*) -> Wrapper<std::string>;   // deduction guide

int main() {
    Pair p(1, 2.5);                 // CTAD: Pair<int, double>
    std::cout << "Pair: " << p.first << ", " << p.second << "\n";

    std::vector v = {1, 2, 3};      // CTAD from the standard library: vector<int>
    std::cout << "vector size: " << v.size() << "\n";

    Wrapper w("hello");             // the guide -> Wrapper<std::string>, not <const char*>
    std::cout << "Wrapper holds a string of length " << w.value.size() << "\n";   // 5
    return 0;
}
