// Anti-pattern: a hidden concrete dependency reached through a static.
//
// PaymentProcessor takes no logger and looks self-contained, but process()
// calls FileLogger::instance() -- a concrete singleton. The dependency is real
// and invisible: the signature hides it, no caller can substitute it, and a test
// cannot avoid the side effect.
//
// The fix injects a Logger abstraction, so the collaborator is explicit and
// replaceable (a ConsoleLogger in production, a silent one in a test).

#include <iostream>
#include <string>

// ---------- policy hard-wired to a concrete singleton ----------
namespace bad {

class FileLogger {
public:
    static FileLogger& instance() {
        static FileLogger logger;
        return logger;
    }
    void log(const std::string& msg) { std::cout << "[file] " << msg << "\n"; }
};

class PaymentProcessor {
public:
    std::string process(long cents) {
        FileLogger::instance().log("charging " + std::to_string(cents)); // hidden dep
        return "charged " + std::to_string(cents);
    }
};

} // namespace bad

// ---------- policy depends on an injected abstraction ----------
namespace good {

class Logger {
public:
    virtual ~Logger() {}
    virtual void log(const std::string& msg) = 0;
};

class ConsoleLogger : public Logger {
public:
    void log(const std::string& msg) override { std::cout << "[console] " << msg << "\n"; }
};

class PaymentProcessor {
public:
    explicit PaymentProcessor(Logger& logger) : logger_(logger) {}
    std::string process(long cents) {
        logger_.log("charging " + std::to_string(cents)); // explicit, replaceable
        return "charged " + std::to_string(cents);
    }
private:
    Logger& logger_;
};

} // namespace good

int main() {
    bad::PaymentProcessor legacy;
    std::cout << "[bad]  " << legacy.process(5000) << "\n";

    good::ConsoleLogger logger;
    good::PaymentProcessor svc(logger);
    std::cout << "[good] " << svc.process(5000) << "\n";
}
