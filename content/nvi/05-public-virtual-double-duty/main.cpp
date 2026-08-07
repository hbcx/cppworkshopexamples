#include <iostream>

int auditCount = 0;   // how many operations were recorded in the audit log

// BAD: process() is public AND virtual, so it is both the interface and the
// customization point. Every override must remember to write the audit record;
// one that forgets silently skips it -- a compliance bug, not a crash.
struct BadHandler {
    virtual ~BadHandler() = default;
    virtual void process() { ++auditCount; }        // base audits; overrides replace this
};
struct BadPayment : BadHandler {
    void process() override { ++auditCount; std::cout << "pay\n"; }          // remembered
};
struct BadRefund : BadHandler {
    void process() override { std::cout << "refund\n"; }   // forgot to audit -- BUG
};

// GOOD: NVI. The public non-virtual handle() writes the audit record every
// time, then calls the private virtual doProcess(). No override can skip it.
struct GoodHandler {
    virtual ~GoodHandler() = default;
    void handle() { ++auditCount; doProcess(); }
private:
    virtual void doProcess() = 0;
};
struct GoodPayment : GoodHandler {
    void doProcess() override { std::cout << "pay\n"; }
};
struct GoodRefund : GoodHandler {
    void doProcess() override { std::cout << "refund\n"; }
};

int main() {
    auditCount = 0;
    BadPayment{}.process();
    BadRefund{}.process();
    std::cout << "bad:  auditCount=" << auditCount << "  <- 1, the refund was not audited\n\n";

    auditCount = 0;
    GoodPayment{}.handle();
    GoodRefund{}.handle();
    std::cout << "good: auditCount=" << auditCount << "  <- 2, every operation audited\n";
    return 0;
}
