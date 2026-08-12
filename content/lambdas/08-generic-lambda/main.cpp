#include <iostream>
#include <string>

int main() {
    // A generic lambda uses auto for a parameter (C++14), which gives it a
    // templated operator(): one lambda works for many argument types, each call
    // instantiating it for the type passed.
    auto show = [](const auto& x) { std::cout << x << '\n'; };
    show(42);
    show(3.14);
    show(std::string("text"));

    // auto&& is a forwarding parameter: it binds to anything and preserves whether
    // the argument was an lvalue or rvalue -- the usual choice for a generic wrapper.
    auto twice = [](auto&& x) { return x + x; };
    std::cout << "twice(21) = " << twice(21) << '\n';    // 42
    std::cout << "twice(1.5) = " << twice(1.5) << '\n';  // 3
    return 0;
}
