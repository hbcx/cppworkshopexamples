#include <iostream>
#include <string>
#include <type_traits>

// An `auto` function parameter (C++20) is SHORTHAND for a template parameter:
// this is exactly the same as template <class T> void show(T x).
void show(auto x) {
    std::cout << "show: " << x << "\n";
}

// It really is a template -- it deduces per call, and each argument type is a
// separate instantiation, just like a written-out function template.
auto twice(auto x) {
    return x + x;
}

int main() {
    show(42);                  // instantiates show<int>
    show(3.14);                // instantiates show<double>
    show(std::string("hi"));   // instantiates show<std::string>

    std::cout << "twice(21)  = " << twice(21) << "\n";     // 42
    std::cout << "twice(2.5) = " << twice(2.5) << "\n";    // 5

    // A generic lambda (C++14) is the same idea one step earlier: [](auto x){...}
    // gives the closure a templated operator(), so one lambda serves many types.
    auto describe = [](auto x) {
        std::cout << "value " << x << " -- arithmetic? " << std::boolalpha
                  << std::is_arithmetic_v<decltype(x)> << "\n";
    };
    describe(7);
    describe(std::string("no"));
    return 0;
}
