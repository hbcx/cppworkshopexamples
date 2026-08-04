// Anti-pattern: one fat handler doing every step.
//
// The value of a chain (or pipeline) is that each handler has ONE responsibility
// and neither knows nor does the next handler's job -- so steps can be reordered,
// reused, tested and inserted independently. A fat handler that crams validate,
// enrich and save into one method throws all of that away: it is a chain of one,
// impossible to recombine, and every change reopens the same class. The fix is to
// split it into focused handlers that each do one step and pass the request on.

#include <iostream>

struct RequestData {
    int value;
    bool valid;
};

// ---- BAD: one handler doing all three steps; nothing is reusable or reorderable ----
class FatHandler {
public:
    void handle(RequestData& r) {
        // step 1: validate
        r.valid = r.value >= 0;
        if (!r.valid) { std::cout << "  rejected\n"; return; }
        // step 2: enrich
        r.value *= 2;
        // step 3: save
        std::cout << "  saved " << r.value << "\n";
    }
};

// ---- GOOD: one focused handler per step, chained; each does its bit and passes on ----
class Step {
    Step* next_ = nullptr;
public:
    virtual ~Step() = default;
    Step* setNext(Step* n) { next_ = n; return n; }
    void handle(RequestData& r) {
        if (!run(r)) return;              // a step may stop the chain
        if (next_) next_->handle(r);
    }
protected:
    virtual bool run(RequestData& r) = 0; // return false to stop
};
class Validate : public Step {
    bool run(RequestData& r) override {
        r.valid = r.value >= 0;
        if (!r.valid) { std::cout << "  rejected\n"; return false; }
        return true;
    }
};
class Enrich : public Step {
    bool run(RequestData& r) override { r.value *= 2; return true; }
};
class Save : public Step {
    bool run(RequestData& r) override { std::cout << "  saved " << r.value << "\n"; return true; }
};

int main() {
    std::cout << "fat handler:\n";
    { RequestData r{ 5, false };  FatHandler{}.handle(r); }
    { RequestData r{ -1, false }; FatHandler{}.handle(r); }

    std::cout << "chained steps:\n";
    Validate v; Enrich e; Save s;
    v.setNext(&e)->setNext(&s); // validate -> enrich -> save
    { RequestData r{ 5, false };  v.handle(r); }
    { RequestData r{ -1, false }; v.handle(r); }
    return 0;
}
