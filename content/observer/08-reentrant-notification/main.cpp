#include <functional>
#include <iostream>
#include <utility>
#include <vector>

// ANTI-PATTERN: an observer reacts by changing the subject again, so notify() runs
// an observer that calls setValue() which runs notify()... a re-entrant cascade.
// Bounded here (v < 3) to stay safe, but with no natural stop it recurses without
// limit and overflows the stack, and even bounded it deepens the call stack and
// interleaves half-applied changes.
class SubjectBad {
public:
    void subscribe(std::function<void(int)> h) { handlers_.push_back(std::move(h)); }
    void setValue(int v) {
        value_ = v;
        for (auto& h : handlers_) h(value_);   // may re-enter setValue through a handler
    }
    int value() const { return value_; }
private:
    int value_ = 0;
    std::vector<std::function<void(int)>> handlers_;
};

// FIX: guard against re-entrancy. While a notification is in progress, setValue
// only records the pending value; the in-flight loop applies it, so a change made
// from an observer is Merged into the running broadcast instead of recursing --
// the call stack stays flat no matter how observers react.
class SubjectGood {
public:
    void subscribe(std::function<void(int)> h) { handlers_.push_back(std::move(h)); }
    void setValue(int v) {
        pending_ = v;
        if (notifying_) return;                // already broadcasting -> just record
        notifying_ = true;
        while (applied_ != pending_) {
            applied_ = pending_;
            for (auto& h : handlers_) h(applied_);
        }
        notifying_ = false;
    }
    int value() const { return applied_; }
private:
    bool notifying_ = false;
    int pending_ = 0;
    int applied_ = 0;
    std::vector<std::function<void(int)>> handlers_;
};

int main() {
    std::cout << "== re-entrant: setValue recurses through the observer ==\n";
    SubjectBad bad;
    bad.subscribe([&bad](int v) {
        std::cout << "bad sees " << v << "\n";
        if (v < 3) bad.setValue(v + 1);        // reacts by changing the subject -> recursion
    });
    bad.setValue(1);
    std::cout << "bad final value: " << bad.value() << "\n";

    std::cout << "== guarded: the change is coalesced, no recursion ==\n";
    SubjectGood good;
    good.subscribe([&good](int v) {
        std::cout << "good sees " << v << "\n";
        if (v < 3) good.setValue(v + 1);       // recorded, applied by the running loop
    });
    good.setValue(1);
    std::cout << "good final value: " << good.value() << "\n";
    return 0;
}
