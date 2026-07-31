#include <iostream>
#include <string>

// Anti-pattern: a global clock singleton reached for directly inside business
// logic. The function that uses it does not declare the dependency and a test
// cannot make it return a fixed time.
class GlobalClock {
public:
    static GlobalClock& instance() {
        static GlobalClock c;
        return c;
    }
    std::string now() const { return "2026-07-31T10:00"; }

    GlobalClock(const GlobalClock&) = delete;
    GlobalClock& operator=(const GlobalClock&) = delete;

private:
    GlobalClock() = default;
};

// The signature promises nothing, but the body secretly needs a clock -- and is
// welded to the one global. There is no seam to substitute a fixed time.
std::string makeReceiptGlobal() {
    return "receipt @ " + GlobalClock::instance().now();
}

// Fix: inject the collaborator behind an interface. The need is now explicit in
// the signature, and any implementation can be passed -- including a test stub.
struct Clock {
    virtual std::string now() const = 0;
    virtual ~Clock() = default;
};
struct SystemClock : Clock {
    std::string now() const override { return "2026-07-31T10:00"; }
};
struct StubClock : Clock {   // a test double: a fixed, controlled time
    std::string now() const override { return "1970-01-01T00:00"; }
};

std::string makeReceipt(const Clock& clock) {
    return "receipt @ " + clock.now();
}

int main() {
    // Global singleton: stuck with the one clock, untestable.
    std::cout << makeReceiptGlobal() << '\n';

    // Injected: production passes the real clock...
    SystemClock system;
    std::cout << makeReceipt(system) << '\n';

    // ...and a unit test passes a stub, pinning the time -- the seam the
    // singleton version does not have.
    StubClock stub;
    std::cout << "test: " << makeReceipt(stub) << '\n';
    return 0;
}
