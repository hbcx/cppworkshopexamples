// Anti-pattern: std::move inside a forwarding function. A forwarding reference
// binds lvalues too, so std::move on it unconditionally casts to rvalue -- even
// when the caller passed an lvalue it still owns. The downstream sink then MOVES
// from the caller's object, gutting it behind the caller's back. Use std::forward,
// which casts to rvalue only when the argument really was one.
#include <iostream>
#include <utility>

struct Tracer {
    Tracer() = default;
    Tracer(const Tracer&)     { std::cout << "  copy (caller's object safe)\n"; }
    Tracer(Tracer&&) noexcept { std::cout << "  move (caller's object gutted!)\n"; }
};

void sink(Tracer) {}

template <typename T> void relay_wrong(T&& x) { sink(std::move(x)); }        // always steals
template <typename T> void relay_right(T&& x) { sink(std::forward<T>(x)); }  // steals only rvalues

int main() {
    Tracer a;
    std::cout << "relay_wrong(lvalue): std::move steals unconditionally\n";
    relay_wrong(a);          // moves from a, which main still owns

    std::cout << "relay_right(lvalue): std::forward keeps it a copy\n";
    relay_right(a);          // copies -- a is untouched
    return 0;
}
