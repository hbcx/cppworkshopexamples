#include <functional>
#include <iostream>

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

int main() {
    using namespace std::placeholders;

    // ANTI-PATTERN: nesting one std::bind inside another to compose functions.
    // bind gives a nested bind SPECIAL treatment -- the inner bind is itself
    // invoked with the outer call arguments and its result passed along -- so the
    // behaviour is easy to misread. This computes add(mul(_1, _1), 1).
    auto composed = std::bind(add, std::bind(mul, _1, _1), 1);
    std::cout << "composed(6) = " << composed(6) << "\n";   // mul(6,6)=36, +1 = 37

    // The trap: the inner bind's placeholders share the OUTER call's arguments,
    // and whether a nested object is "a bind to invoke" or "a value to pass"
    // depends on its type. That is subtle and gets worse as the nesting grows.

    // FIX: a lambda spells the composition out; the order of evaluation is plain
    // and nothing is silently re-invoked.
    auto clear = [](int x){ return add(mul(x, x), 1); };
    std::cout << "clear(6)    = " << clear(6) << "\n";       // 37
    return 0;
}
