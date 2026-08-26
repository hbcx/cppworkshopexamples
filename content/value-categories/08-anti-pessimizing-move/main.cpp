// Anti-pattern: std::move on the return value. Writing `return std::move(local);`
// looks like an optimization but is the opposite: it BLOCKS copy elision (NRVO).
// A plain `return local;` lets the compiler build the result directly in the
// caller -- no copy and no move at all -- and if it cannot, it still moves. Adding
// std::move forces a move where you could have had nothing. Just return the name.
#include <iostream>
#include <string>
#include <utility>

struct Tracer {
    Tracer() = default;
    Tracer(const Tracer&)     { std::cout << "  copied\n"; }
    Tracer(Tracer&&) noexcept { std::cout << "  moved\n"; }
};

Tracer good() {
    Tracer t;
    return t;                 // eligible for NRVO: usually no copy AND no move
}

Tracer bad() {
    Tracer t;
    return std::move(t);      // forces a move; elision can no longer apply
}

int main() {
    std::cout << "return t;\n";
    Tracer a = good();        // typically prints nothing (elided)
    std::cout << "return std::move(t);\n";
    Tracer b = bad();         // prints "moved"

    (void)a;
    (void)b;
    return 0;
}
