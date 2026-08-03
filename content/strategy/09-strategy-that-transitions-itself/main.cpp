#include <iostream>

// ANTI-PATTERN: a "strategy" chosen by the client whose operation ALSO changes
// which strategy the context uses next. Strategy promises an interchangeable
// algorithm the client selects and that stays put; here the client's choice
// silently advances, and each step is coupled to the next. Behaviour that depends
// on and changes with the object's own progress is State, not Strategy.
struct Ctx;
struct Step {
    virtual void run(Ctx& c) const = 0;
    virtual ~Step() = default;
};

struct Ctx {
    const Step* step;
    void process();
};

struct Placed;
struct Shipped;
const Step& placedStep();
const Step& shippedStep();

struct Placed : Step {
    void run(Ctx& c) const override;
};
struct Shipped : Step {
    void run(Ctx& /*c*/) const override { std::cout << "shipped\n"; }
};

const Step& placedStep()  { static const Placed s;  return s; }
const Step& shippedStep() { static const Shipped s; return s; }

void Placed::run(Ctx& c) const {
    std::cout << "placed, advancing to shipped\n";
    c.step = &shippedStep();          // the strategy reassigns the context's strategy -- that is State
}
void Ctx::process() { step->run(*this); }

// FIX: model it as State -- the object owns its status and its transitions, and
// the client never picks the state; it just drives the object, which advances
// itself. (State is the pattern for behaviour tied to an object's own status; with
// many states it uses state objects rather than a flag, but the point is that the
// transitions are internal and not exposed as a client-selected strategy.)
class Order {
public:
    void process() {
        if (!shipped_) {
            std::cout << "placed, advancing to shipped\n";
            shipped_ = true;
        } else {
            std::cout << "shipped\n";
        }
    }
private:
    bool shipped_ = false;   // private state; only Order changes it
};

int main() {
    std::cout << "== strategy that transitions itself (really State) ==\n";
    Ctx ctx{&placedStep()};
    ctx.process();                                  // placed, advancing to shipped
    ctx.process();                                  // shipped
    const bool stuck = (ctx.step == &placedStep()); // did the client's chosen step stay?
    std::cout << "client's chosen step stayed put? " << (stuck ? "yes" : "no, it advanced") << "\n";

    std::cout << "== modelled as State ==\n";
    Order order;
    order.process();                                // placed, advancing to shipped
    order.process();                                // shipped
    return 0;
}
