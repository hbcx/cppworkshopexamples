// Anti-pattern: a shared state object that hides per-context data.
//
// State objects are shared freely -- one Locked, one Playing -- precisely because
// a state usually has no data of its own. Put mutable per-context data in a state
// and share it, and the contexts bleed into each other: they hold the same
// instance, so one context's calls change what the others see. Here two machines
// share one state that keeps a running count; because the count lives in the
// shared object, machine b's result depends on machine a's earlier calls. The fix
// is to keep states stateless and put per-context data in the context.

#include <iostream>

// ---------- BAD: per-context data living in a shared state ----------
struct CountingStateBad {
    int hits_ = 0;                 // WRONG: this belongs to a context, not the shared state
    void handle(const char* who) { // non-const: mutates the shared object
        ++hits_;
        std::cout << who << " sees hits=" << hits_ << "\n";
    }
};

class MachineBad {
    CountingStateBad* state_;      // shared with other machines
public:
    explicit MachineBad(CountingStateBad* s) : state_(s) {}
    void run(const char* who) { state_->handle(who); }
};

// ---------- GOOD: stateless shared state, per-context data in the context ----------
struct CountingStateGood {
    void handle(const char* who, int hits) const { // const: never mutates itself
        std::cout << who << " sees hits=" << hits << "\n";
    }
};

class MachineGood {
    const CountingStateGood* state_; // shared, but stateless -> safe
    int hits_ = 0;                   // per-context data lives HERE
public:
    explicit MachineGood(const CountingStateGood* s) : state_(s) {}
    void run(const char* who) { ++hits_; state_->handle(who, hits_); }
};

int main() {
    std::cout << "--- bad (entangled) ---\n";
    CountingStateBad sharedBad;
    MachineBad a(&sharedBad), b(&sharedBad);
    a.run("a"); // hits=1
    a.run("a"); // hits=2
    b.run("b"); // hits=3  <- b inherited a's count through the shared state

    std::cout << "--- good (independent) ---\n";
    CountingStateGood sharedGood;
    MachineGood c(&sharedGood), d(&sharedGood);
    c.run("c"); // hits=1
    c.run("c"); // hits=2
    d.run("d"); // hits=1  <- d has its own count
    return 0;
}
