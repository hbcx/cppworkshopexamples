#include <iostream>
#include <string>

// The dependency, as an interface. The consumer depends on THIS, never on a
// concrete implementation.
struct Notifier {
    virtual void send(const std::string& msg) = 0;
    virtual ~Notifier() = default;
};

struct ConsoleNotifier : Notifier {
    void send(const std::string& msg) override {
        std::cout << "[console] " << msg << '\n';
    }
};

struct EmailNotifier : Notifier {
    void send(const std::string& msg) override {
        std::cout << "[email] to ops: " << msg << '\n';
    }
};

// The consumer RECEIVES its dependency through the constructor instead of
// constructing one itself. It names no concrete Notifier, so it works with any
// implementation, and the caller decides which.
class Alarm {
public:
    explicit Alarm(Notifier& notifier) : notifier_(notifier) {}
    void trip(const std::string& reason) {
        notifier_.send("ALARM: " + reason);
    }
private:
    Notifier& notifier_;   // non-owning: the caller owns the dependency
};

int main() {
    ConsoleNotifier console;
    EmailNotifier email;

    // The same Alarm class, two different dependencies injected -- no change to
    // Alarm; the wiring picks the behaviour.
    Alarm devAlarm(console);
    devAlarm.trip("disk almost full");

    Alarm prodAlarm(email);
    prodAlarm.trip("disk almost full");
    return 0;
}
