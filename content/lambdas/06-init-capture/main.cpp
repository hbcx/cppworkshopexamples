#include <iostream>
#include <memory>
#include <string>
#include <utility>

int main() {
    // Init-capture (C++14): the capture introduces a NEW variable, initialized by
    // an expression, instead of just copying an existing one. Three common uses:

    // 1. Compute a value once, at capture time, and store the result.
    int width = 4, height = 5;
    auto area = [a = width * height] { return a; };
    std::cout << "captured area: " << area() << '\n';   // 20

    // 2. Capture an expression that is not a plain variable, or rename it.
    auto greet = [name = std::string("Ada")] { return "hello " + name; };
    std::cout << greet() << '\n';

    // 3. The important one: capture BY MOVE. A unique_ptr cannot be copied into a
    // closure, but init-capture can move it in.
    auto up = std::make_unique<int>(42);
    auto owns = [p = std::move(up)] { return *p; };
    std::cout << "moved-in value: " << owns() << '\n';   // 42
    std::cout << "original pointer is now " << (up ? "set" : "null") << '\n';   // null
    return 0;
}
