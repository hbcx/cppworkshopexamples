// std::forward<T>(x) casts x back to the value category it arrived as: if the
// caller passed an lvalue, forward yields an lvalue (a copy is made downstream);
// if an rvalue, forward yields an rvalue (a move). Inside a forwarding-reference
// function, x itself is always an lvalue (it has a name), so you MUST forward to
// pass the original category on. std::forward is a conditional cast.
#include <iostream>
#include <utility>

struct Tracer {
    Tracer() = default;
    Tracer(const Tracer&)     { std::cout << "  copy\n"; }
    Tracer(Tracer&&) noexcept { std::cout << "  move\n"; }
};

void sink(Tracer) {}                  // by value: the parameter is copied or moved

template <typename T>
void relay(T&& x) {
    sink(std::forward<T>(x));          // preserve the caller's category
}

int main() {
    Tracer t;
    std::cout << "relay(lvalue):\n";
    relay(t);                          // lvalue -> copy
    std::cout << "relay(rvalue):\n";
    relay(Tracer());                   // rvalue -> move
    return 0;
}
