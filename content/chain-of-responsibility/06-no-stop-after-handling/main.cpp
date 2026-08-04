// Anti-pattern: forwarding even after handling (no short-circuit).
//
// In the pure chain, exactly one handler should process a request and the
// traversal should then stop. If a handler processes the request but forgets to
// return -- and forwards anyway -- every downstream handler that also matches
// will process it too. With overlapping predicates this means one event is
// handled several times. Here a severity-9 event matches the critical, warning
// AND info handlers, so the buggy chain acts on it three times. The fix is to
// stop after handling.

#include <iostream>

// ---- BAD: handles, then forwards regardless -> multiple handling ----
class LoggerBad {
    LoggerBad* next_ = nullptr;
public:
    virtual ~LoggerBad() = default;
    LoggerBad* setNext(LoggerBad* n) { next_ = n; return n; }
    void handle(int severity) {
        if (accepts(severity)) emit(severity); // no return...
        if (next_) next_->handle(severity);    // ...so it always forwards too
    }
protected:
    virtual bool accepts(int) const = 0;
    virtual void emit(int) const = 0;
};
class CriticalBad : public LoggerBad {
    bool accepts(int s) const override { return s >= 8; }
    void emit(int) const override { std::cout << "  PAGE on-call\n"; }
};
class WarningBad : public LoggerBad {
    bool accepts(int s) const override { return s >= 5; }
    void emit(int) const override { std::cout << "  log warning\n"; }
};
class InfoBad : public LoggerBad {
    bool accepts(int) const override { return true; }
    void emit(int) const override { std::cout << "  log info\n"; }
};

// ---- GOOD: stop after handling ----
class LoggerGood {
    LoggerGood* next_ = nullptr;
public:
    virtual ~LoggerGood() = default;
    LoggerGood* setNext(LoggerGood* n) { next_ = n; return n; }
    void handle(int severity) {
        if (accepts(severity)) { emit(severity); return; } // handled -> stop
        if (next_) next_->handle(severity);
    }
protected:
    virtual bool accepts(int) const = 0;
    virtual void emit(int) const = 0;
};
class CriticalGood : public LoggerGood {
    bool accepts(int s) const override { return s >= 8; }
    void emit(int) const override { std::cout << "  PAGE on-call\n"; }
};
class WarningGood : public LoggerGood {
    bool accepts(int s) const override { return s >= 5; }
    void emit(int) const override { std::cout << "  log warning\n"; }
};
class InfoGood : public LoggerGood {
    bool accepts(int) const override { return true; }
    void emit(int) const override { std::cout << "  log info\n"; }
};

int main() {
    CriticalBad cb; WarningBad wb; InfoBad ib;
    cb.setNext(&wb)->setNext(&ib);
    std::cout << "bad chain, severity 9:\n"; cb.handle(9); // handled three times

    CriticalGood cg; WarningGood wg; InfoGood ig;
    cg.setNext(&wg)->setNext(&ig);
    std::cout << "good chain, severity 9:\n"; cg.handle(9); // handled once
    return 0;
}
