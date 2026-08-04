// Anti-pattern: a state that deletes itself mid-transition (use-after-free).
//
// The textbook way to transition is to have the current state call
// context.setState(next) from inside its own handler. If setState destroys the
// old state immediately, that is a use-after-free: the destroyed object is the
// one whose method is still running, so every line after the call -- including
// the implicit 'this' -- touches freed memory. The bad version is described in
// the comment below and deliberately NOT run (its behaviour is undefined). The
// fix, which this file runs, is to have the handler RETURN the next state and
// let the context swap after the handler has fully returned.
//
// ---- BAD (undefined behaviour -- shown, not executed) ----
//
//   struct StateBad {
//       std::string label_ = "A";
//       void handle(ContextBad& c) {
//           c.setState(new StateBad{"B"}); // setState deletes *this...
//           std::cout << label_;           // ...label_ is now freed memory: UAF
//       }
//   };
//   class ContextBad {
//       StateBad* s_;
//   public:
//       void setState(StateBad* n) { delete s_; s_ = n; } // destroys the running state
//   };
//
// The transition is triggered from inside the state's own method, and the
// context deletes the old state right away, so control returns into an object
// that no longer exists. It often "works" by luck, which is what makes it
// dangerous. Never destroy the current state while its method is on the stack.

#include <iostream>
#include <memory>

// ---- GOOD: the handler returns the next state; the context swaps afterwards ----
struct State {
    virtual ~State() = default;
    virtual std::unique_ptr<State> handle() = 0; // do all work, then return next
    virtual const char* name() const = 0;
};

struct StateB : State {
    std::unique_ptr<State> handle() override {
        std::cout << "B handled, staying\n";
        return nullptr; // no transition
    }
    const char* name() const override { return "B"; }
};

struct StateA : State {
    std::unique_ptr<State> handle() override {
        std::cout << "A handled, moving to B\n"; // all work done BEFORE returning
        return std::unique_ptr<State>(new StateB());
    }
    const char* name() const override { return "A"; }
};

class Context {
    std::unique_ptr<State> s_;
public:
    Context() : s_(new StateA()) {}
    void step() {
        auto next = s_->handle();       // the old state finishes its method first...
        if (next) s_ = std::move(next); // ...then it is destroyed out here, safely
    }
    const char* name() const { return s_->name(); }
};

int main() {
    Context c;
    std::cout << "start: " << c.name() << "\n";
    c.step(); // A -> B, old A destroyed only after handle() returned
    std::cout << "now:   " << c.name() << "\n";
    c.step(); // B stays
    std::cout << "end:   " << c.name() << "\n";
    // (Shared singleton states are the other fix: with nothing to delete on a
    // transition, reassigning the pointer is safe from anywhere -- see example 03.)
    return 0;
}
