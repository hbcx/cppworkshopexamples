#include <iostream>

// ANTI-PATTERN: the strategy holds MUTABLE state (a running total) and is SHARED
// by several contexts. Because they share one instance, each context's calls
// pollute the others' state -- the accumulator mixes results that were supposed to
// be independent. A strategy meant to be shared must be stateless.
class AccumulatingStrategyBad {
public:
    int apply(int value) { sum_ += value; return sum_; }   // per-use state on a shared object
private:
    int sum_ = 0;
};

class Context {
public:
    explicit Context(AccumulatingStrategyBad& s) : strategy_(&s) {}
    int run(int value) { return strategy_->apply(value); }
private:
    AccumulatingStrategyBad* strategy_;
};

// FIX: the strategy is stateless -- it computes from its input alone, so any
// number of contexts can share one instance with no cross-talk. State that must
// persist belongs in the context, not in the shared strategy.
struct StatelessStrategy {
    int apply(int value) const { return value * 2; }
};

class ContextGood {
public:
    explicit ContextGood(const StatelessStrategy& s) : strategy_(&s) {}
    int run(int value) const { return strategy_->apply(value); }
private:
    const StatelessStrategy* strategy_;
};

int main() {
    std::cout << "== mutable strategy shared by two contexts ==\n";
    AccumulatingStrategyBad shared;
    Context a(shared);
    Context b(shared);
    std::cout << "a.run(10) = " << a.run(10) << "\n";   // 10
    std::cout << "b.run(10) = " << b.run(10) << "\n";   // 20 -- polluted by a's call

    std::cout << "== stateless strategy shared safely ==\n";
    StatelessStrategy stateless;
    ContextGood c(stateless);
    ContextGood d(stateless);
    std::cout << "c.run(10) = " << c.run(10) << "\n";   // 20
    std::cout << "d.run(10) = " << d.run(10) << "\n";   // 20 -- independent
    return 0;
}
