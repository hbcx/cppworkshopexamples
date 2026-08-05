// SRP: separate the decision (policy) from the I/O (mechanism).
//
// A monitor that decides an alert level AND writes the alert has two jobs. The
// rule is a product decision; the output destination is an operations decision.
// Welding them also makes the rule untestable -- you cannot check what it
// decides without triggering the side effect.
//
// Split: a pure ThresholdPolicy that returns a Level with no I/O, and an
// AlertSink that turns a Level into output. The policy is now checkable in
// memory; the sink is swappable without touching the rule.

#include <iostream>
#include <string>

enum class Level { Ok, Warn, Critical };

static std::string name(Level l) {
    switch (l) {
        case Level::Ok:       return "OK";
        case Level::Warn:     return "WARN";
        case Level::Critical: return "CRITICAL";
    }
    return "?";
}

// Pure policy: a number in, a decision out. No printing, no files.
class ThresholdPolicy {
public:
    ThresholdPolicy(double warnAt, double critAt) : warn_(warnAt), crit_(critAt) {}

    Level evaluate(double celsius) const {
        if (celsius >= crit_) return Level::Critical;
        if (celsius >= warn_) return Level::Warn;
        return Level::Ok;
    }

private:
    double warn_;
    double crit_;
};

// Mechanism: turn a decision into output. Console today, a file or pager
// tomorrow -- swapping it never touches the rule above.
class AlertSink {
public:
    void report(const std::string& sensor, double celsius, Level lvl) const {
        std::cout << "[" << name(lvl) << "] " << sensor << " at "
                  << celsius << "C\n";
    }
};

int main() {
    ThresholdPolicy policy(30.0, 45.0);

    // Because the rule is pure, it can be unit-tested with no I/O at all.
    std::cout << "self-test: "
              << (policy.evaluate(20.0) == Level::Ok       ? "ok "  : "FAIL ")
              << (policy.evaluate(35.0) == Level::Warn     ? "warn " : "FAIL ")
              << (policy.evaluate(50.0) == Level::Critical ? "crit"  : "FAIL")
              << "\n";

    // Wire policy + sink for a real run.
    AlertSink sink;
    const double readings[] = {22.5, 38.0, 47.0};
    for (double c : readings)
        sink.report("boiler-1", c, policy.evaluate(c));
}
