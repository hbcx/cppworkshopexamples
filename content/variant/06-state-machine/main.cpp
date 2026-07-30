#include <iostream>
#include <string>
#include <variant>

// Each state is a struct carrying its own data. The machine's state is a variant
// of them -- a closed set, value semantics, no heap -- where a class hierarchy
// would need virtual dispatch and allocation.
struct Idle {};
struct Connecting { int attempts; };
struct Connected { std::string peer; };

using State = std::variant<Idle, Connecting, Connected>;

// A "connect succeeded" event: one operator() per state, each returning the next
// state. visit is exhaustive, so a new state forces a handler here.
struct OnSuccess {
    State operator()(Idle) const { return Connecting{1}; }
    State operator()(Connecting c) const { return Connected{"server-" + std::to_string(c.attempts)}; }
    State operator()(const Connected& c) const { return c; }   // already connected
};

// Print the current state.
struct Describe {
    void operator()(Idle) const { std::cout << "Idle\n"; }
    void operator()(Connecting c) const { std::cout << "Connecting (attempt " << c.attempts << ")\n"; }
    void operator()(const Connected& c) const { std::cout << "Connected to " << c.peer << '\n'; }
};

int main() {
    State s = Idle{};
    for (int i = 0; i < 3; ++i) {
        std::visit(Describe{}, s);
        s = std::visit(OnSuccess{}, s);
    }
    std::visit(Describe{}, s);
    return 0;
}
