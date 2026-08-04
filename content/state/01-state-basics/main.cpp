// State basics: a turnstile whose behaviour depends on its current state.
//
// The mechanic: the context (Turnstile) does not decide anything itself -- it
// holds a State object and forwards each event to it. Each state handles the
// event and returns the NEXT state (or nullptr to stay put), so the transitions
// live inside the states, not in a switch in the context. Returning the next
// state -- rather than having the state reassign the context mid-call -- keeps
// the swap safe: the old state is destroyed after its method has returned.

#include <iostream>
#include <memory>

struct State {
    virtual ~State() = default;
    // Each handler returns the state to move to, or nullptr to stay.
    virtual std::unique_ptr<State> coin() = 0;
    virtual std::unique_ptr<State> push() = 0;
    virtual const char* name() const = 0;
};

struct Unlocked; // forward: Locked transitions to it

struct Locked : State {
    std::unique_ptr<State> coin() override; // pay -> unlock
    std::unique_ptr<State> push() override; // denied, stay locked
    const char* name() const override { return "Locked"; }
};

struct Unlocked : State {
    std::unique_ptr<State> coin() override; // already paid, stay
    std::unique_ptr<State> push() override; // go through -> lock behind
    const char* name() const override { return "Unlocked"; }
};

std::unique_ptr<State> Locked::coin() {
    std::cout << "coin  -> unlock\n";
    return std::unique_ptr<State>(new Unlocked());
}
std::unique_ptr<State> Locked::push() {
    std::cout << "push  -> denied, pay first\n";
    return nullptr;
}
std::unique_ptr<State> Unlocked::coin() {
    std::cout << "coin  -> thanks, already unlocked\n";
    return nullptr;
}
std::unique_ptr<State> Unlocked::push() {
    std::cout << "push  -> go through, lock behind you\n";
    return std::unique_ptr<State>(new Locked());
}

class Turnstile {
    std::unique_ptr<State> state_;
public:
    Turnstile() : state_(new Locked()) {}
    void coin() { if (auto next = state_->coin()) state_ = std::move(next); }
    void push() { if (auto next = state_->push()) state_ = std::move(next); }
    const char* state() const { return state_->name(); }
};

int main() {
    Turnstile t;
    std::cout << "start: " << t.state() << "\n";
    t.push(); // denied while Locked
    t.coin(); // -> Unlocked
    std::cout << "now:   " << t.state() << "\n";
    t.push(); // -> Locked
    std::cout << "now:   " << t.state() << "\n";
    t.coin(); // -> Unlocked
    t.coin(); // already unlocked, no change
    std::cout << "end:   " << t.state() << "\n";
    return 0;
}
