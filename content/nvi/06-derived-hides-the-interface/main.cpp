#include <iostream>

int checks = 0;   // how many times the required safety check ran

// The NVI base: publish() is non-virtual and runs a required check before the
// virtual step.
struct Publisher {
    virtual ~Publisher() = default;
    void publish() { ++checks; doPublish(); }        // required check in the wrapper
private:
    virtual void doPublish() = 0;
};

// BAD: this subclass declares its OWN publish(), which HIDES the base's
// non-virtual wrapper. A call through a Draft runs this one and skips the check.
struct Draft : Publisher {
    void publish() { std::cout << "draft published (no check!)\n"; }   // hides the base
private:
    void doPublish() override { std::cout << "draft body\n"; }
};

// GOOD: this subclass overrides only the virtual step and leaves the wrapper
// alone, so the check always runs.
struct Article : Publisher {
private:
    void doPublish() override { std::cout << "article body\n"; }
};

int main() {
    checks = 0;
    Draft{}.publish();       // Draft::publish -> base check SKIPPED
    std::cout << "bad:  checks=" << checks << "  <- 0, the wrapper was bypassed\n\n";

    checks = 0;
    Article{}.publish();     // Publisher::publish -> check runs, then doPublish()
    std::cout << "good: checks=" << checks << "  <- 1, wrapper ran\n";
    return 0;
}
