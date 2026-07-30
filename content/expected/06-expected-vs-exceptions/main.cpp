#include <expected>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

// Same fallible operation, two error strategies.

// (1) Exceptions: the failure is out-of-band; the signature looks infallible.
int divideThrow(int a, int b) {
    if (b == 0) throw std::invalid_argument("divide by zero");
    return a / b;
}

// (2) expected: the failure is in the return type; the caller must acknowledge it.
std::expected<int, std::string> divideExpected(int a, int b) {
    if (b == 0) return std::unexpected("divide by zero");
    return a / b;
}

int main() {
    // Exception path: nothing at the call site says this can fail; you must know
    // to wrap it, and forgetting means the throw escapes.
    try {
        int ok = divideThrow(10, 2);
        std::cout << "throw: " << ok << '\n';
        int bad = divideThrow(1, 0);   // throws before anything is printed
        std::cout << "throw: " << bad << '\n';
    } catch (const std::exception& e) {
        std::cout << "throw: caught -- " << e.what() << '\n';
    }

    // expected path: the failure is visible in the type and handled inline, with
    // no stack unwinding -- a better fit when failure is common.
    for (auto [a, b] : {std::pair{10, 2}, std::pair{1, 0}}) {
        auto r = divideExpected(a, b);
        if (r) std::cout << "expected: " << *r << '\n';
        else   std::cout << "expected: " << r.error() << '\n';
    }
    return 0;
}
