#include <functional>
#include <iostream>

int triple(int x) { return x * 3; }

int main() {
    // std::function<R(Args...)> is a value that holds any callable matching the
    // signature R(Args...). Here: something taking an int and returning an int.
    std::function<int(int)> f = triple;          // from a free function
    std::cout << "f(4)  = " << f(4) << "\n";      // call it like a function: 12

    // Reassign it to a lambda with the same signature -- same variable, a
    // different callable inside.
    f = [](int x) { return x + 100; };
    std::cout << "f(4)  = " << f(4) << "\n";      // 104

    // It is an ordinary value: copy it, pass it, return it.
    std::function<int(int)> g = f;
    std::cout << "g(4)  = " << g(4) << "\n";      // 104
    return 0;
}
