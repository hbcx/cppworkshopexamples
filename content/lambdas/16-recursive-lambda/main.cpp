#include <iostream>
#include <functional>

int main() {
    // A lambda has no name inside its own body, so it cannot call itself directly.
    // C++23 solves this with the explicit object parameter (deducing this): the
    // first parameter, this auto self, is the closure itself, so self(...) recurses.
    auto factorial = [](this auto self, int n) -> long {
        return n <= 1 ? 1 : n * self(n - 1);
    };
    std::cout << "factorial(6) = " << factorial(6) << '\n';   // 720

    // Before C++23 the common way is a std::function the lambda captures by
    // reference, giving the closure a name to recurse through -- at the cost of
    // type erasure and an indirect call.
    std::function<long(int)> fib = [&fib](int n) -> long {
        return n < 2 ? n : fib(n - 1) + fib(n - 2);
    };
    std::cout << "fib(10) = " << fib(10) << '\n';   // 55
    return 0;
}
