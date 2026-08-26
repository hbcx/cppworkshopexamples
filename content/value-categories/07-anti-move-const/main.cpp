// Anti-pattern: std::move on a const object. std::move only casts to an rvalue --
// it does not remove const. A const lvalue turns into a const rvalue, which the
// move constructor (taking a non-const T&&) cannot bind to, so overload
// resolution falls back to the COPY constructor. The move you intended silently
// becomes a copy, with no error. Do not declare const what you mean to move out of.
#include <iostream>
#include <string>
#include <utility>

// A type that reports whether it was moved or copied.
struct Tracer {
    Tracer() = default;
    Tracer(const Tracer&)     { std::cout << "  copied\n"; }
    Tracer(Tracer&&) noexcept { std::cout << "  moved\n"; }
};

int main() {
    Tracer m;
    const Tracer c;

    std::cout << "from a non-const source:\n";
    Tracer a = std::move(m);      // binds T&& -> moves

    std::cout << "from a const source:\n";
    Tracer b = std::move(c);      // const rvalue cannot bind T&& -> silently copies

    (void)a;
    (void)b;
    return 0;
}
