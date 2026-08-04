// Anti-pattern: a request that falls off the end of the chain unhandled.
//
// If no handler in the chain accepts a request and there is no terminal handler,
// the request reaches the end and simply vanishes: the last handler's next_ is
// null, the loop ends, and nothing happened -- but the sender got no error, so it
// believes the request was handled. Silent drops are among the worst failures
// because there is no trace. The fix is to make the end of the chain a decision:
// a terminal branch (or a default catch-all handler) that reports the miss.

#include <iostream>

struct Ticket { int code; };

// ---- BAD: no terminal; an unmatched ticket disappears with no output ----
class HandlerBad {
    HandlerBad* next_ = nullptr;
public:
    virtual ~HandlerBad() = default;
    HandlerBad* setNext(HandlerBad* n) { next_ = n; return n; }
    void handle(const Ticket& t) {
        if (accepts(t)) { process(t); return; }
        if (next_) next_->handle(t);
        // else: nothing. The ticket is silently dropped.  <-- BUG
    }
protected:
    virtual bool accepts(const Ticket&) const = 0;
    virtual void process(const Ticket&) const = 0;
};
class BillingBad : public HandlerBad {
    bool accepts(const Ticket& t) const override { return t.code == 1; }
    void process(const Ticket&) const override { std::cout << "  billing handled\n"; }
};
class TechBad : public HandlerBad {
    bool accepts(const Ticket& t) const override { return t.code == 2; }
    void process(const Ticket&) const override { std::cout << "  tech handled\n"; }
};

// ---- GOOD: a terminal branch reports anything that falls off the end ----
class HandlerGood {
    HandlerGood* next_ = nullptr;
public:
    virtual ~HandlerGood() = default;
    HandlerGood* setNext(HandlerGood* n) { next_ = n; return n; }
    void handle(const Ticket& t) {
        if (accepts(t)) { process(t); return; }
        if (next_) next_->handle(t);
        else std::cout << "  no handler for code " << t.code << " (sent to dead-letter)\n";
    }
protected:
    virtual bool accepts(const Ticket&) const = 0;
    virtual void process(const Ticket&) const = 0;
};
class BillingGood : public HandlerGood {
    bool accepts(const Ticket& t) const override { return t.code == 1; }
    void process(const Ticket&) const override { std::cout << "  billing handled\n"; }
};
class TechGood : public HandlerGood {
    bool accepts(const Ticket& t) const override { return t.code == 2; }
    void process(const Ticket&) const override { std::cout << "  tech handled\n"; }
};

int main() {
    BillingBad bb; TechBad tb;
    bb.setNext(&tb);
    std::cout << "bad chain, code 1:\n"; bb.handle({ 1 });
    std::cout << "bad chain, code 9:\n"; bb.handle({ 9 }); // no output -> silent drop

    BillingGood bg; TechGood tg;
    bg.setNext(&tg);
    std::cout << "good chain, code 1:\n"; bg.handle({ 1 });
    std::cout << "good chain, code 9:\n"; bg.handle({ 9 }); // reported, not lost
    return 0;
}
