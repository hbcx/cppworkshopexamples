#include <iostream>
#include <utility>

int copies = 0;
struct Tracked {
    Tracked() = default;
    Tracked(const Tracked&) { ++copies; }
    Tracked(Tracked&&) noexcept = default;
};

void sink(Tracked) {}   // consumes a Tracked by value

// BAD: taking the pack BY VALUE. Each parameter is an lvalue here, so passing it
// on copies -- the value category the caller had is lost.
template <class... Ts>
void badForward(Ts... args) {
    (sink(args), ...);        // args are lvalues -> copied into sink
}

// GOOD: forwarding references + std::forward preserve value category, so an
// rvalue is moved on, not copied.
template <class... Ts>
void goodForward(Ts&&... args) {
    (sink(std::forward<Ts>(args)), ...);   // rvalues stay rvalues -> moved
}

int main() {
    copies = 0;
    badForward(Tracked{}, Tracked{});
    std::cout << "by value:  " << copies << " copies\n";   // 2

    copies = 0;
    goodForward(Tracked{}, Tracked{});
    std::cout << "forwarded: " << copies << " copies\n";   // 0
    return 0;
}
