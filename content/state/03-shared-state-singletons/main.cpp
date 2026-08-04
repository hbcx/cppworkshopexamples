// Stateless states are shared: one instance of each, not one per transition.
//
// A state that holds no per-context data is the same for every context and
// every transition, so allocating a fresh one each time (as the basics example
// does) is waste. Make each state a single shared instance -- a Flyweight -- and
// a transition becomes a pointer reassignment with zero allocation. Here two
// turnstiles run through many transitions, yet only two state objects are ever
// constructed, which the construction counter proves.

#include <iostream>

struct State {
    virtual ~State() = default;
    // Return the shared next state, or nullptr to stay. const: states are read-only.
    virtual const State* coin() const = 0;
    virtual const State* push() const = 0;
    virtual const char* name() const = 0;
};

int g_constructed = 0; // how many concrete state objects have ever been built

const State& locked();
const State& unlocked();

struct Locked : State {
    Locked() { ++g_constructed; }
    const State* coin() const override { return &unlocked(); }
    const State* push() const override { return nullptr; } // denied, stay
    const char* name() const override { return "Locked"; }
};

struct Unlocked : State {
    Unlocked() { ++g_constructed; }
    const State* coin() const override { return nullptr; } // already unlocked
    const State* push() const override { return &locked(); }
    const char* name() const override { return "Unlocked"; }
};

// The shared singletons: constructed once, on first use.
const State& locked()   { static Locked s;   return s; }
const State& unlocked() { static Unlocked s; return s; }

class Turnstile {
    const State* state_ = &locked(); // non-owning: points at a shared singleton
public:
    void coin() { if (auto n = state_->coin()) state_ = n; }
    void push() { if (auto n = state_->push()) state_ = n; }
    const char* state() const { return state_->name(); }
};

int main() {
    Turnstile a, b;
    // Drive both turnstiles through several transitions each.
    a.coin(); a.push(); a.coin(); a.push();
    b.coin(); b.coin(); b.push();
    std::cout << "a: " << a.state() << "\n";
    std::cout << "b: " << b.state() << "\n";
    // Despite two turnstiles and many transitions, the states are shared:
    std::cout << "state objects ever constructed: " << g_constructed << "\n";
    return 0;
}
