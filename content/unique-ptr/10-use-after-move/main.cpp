#include <iostream>
#include <memory>
#include <utility>

// Anti-pattern: using a unique_ptr after it has been moved from. std::move
// leaves the source empty (null). Dereferencing a null unique_ptr is undefined
// behaviour, so we OBSERVE the emptiness (defined) instead of dereferencing it.
struct Widget {
    int value;
    explicit Widget(int v) : value(v) {}
};

// A sink that takes ownership and (here) just hands it back.
static std::unique_ptr<Widget> passThrough(std::unique_ptr<Widget> w) {
    return w;
}

int main() {
    std::unique_ptr<Widget> p(new Widget(10));
    std::unique_ptr<Widget> q = std::move(p);    // p is now empty

    // std::cout << p->value;   // BUG: p is null -> dereferencing it is UB.
    std::cout << "p after move: " << (p ? "usable" : "empty (null)") << '\n';
    std::cout << "q holds value: " << q->value << '\n';

    // The subtle version: move into a call, then reach for the source again.
    std::unique_ptr<Widget> r = passThrough(std::move(q));   // q now empty too
    std::cout << "q after being passed: " << (q ? "usable" : "empty (null)") << '\n';
    std::cout << "r holds value: " << r->value << '\n';

    // The fix when you really do want to reuse the variable: give it a new value.
    p.reset(new Widget(99));
    std::cout << "p after reset: " << p->value << '\n';
    return 0;
}
