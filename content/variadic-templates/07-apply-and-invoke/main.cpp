#include <functional>
#include <iostream>
#include <string>
#include <tuple>

int add3(int a, int b, int c) { return a + b + c; }

struct Greeter {
    std::string prefix;
    std::string greet(const std::string& name) const { return prefix + name; }
};

int main() {
    // std::apply UNPACKS a tuple into a call: the elements become the arguments.
    auto args = std::make_tuple(1, 2, 3);
    std::cout << "apply(add3, {1,2,3}) = " << std::apply(add3, args) << "\n";   // 6

    // std::invoke calls ANY callable uniformly: function, lambda, functor, or a
    // pointer-to-member (with the object as the first argument).
    std::cout << "invoke(add3, 4,5,6)  = " << std::invoke(add3, 4, 5, 6) << "\n";   // 15

    Greeter g{"Hello, "};
    // Pointer-to-member-function: invoke passes the object, then the arguments.
    std::cout << "invoke member        = " << std::invoke(&Greeter::greet, g, "world") << "\n";  // Hello, world

    // apply and invoke compose: apply a tuple of args to a lambda.
    std::cout << "apply over a lambda  = "
              << std::apply([](int a, int b){ return a * b; }, std::make_tuple(6, 7)) << "\n";   // 42
    return 0;
}
