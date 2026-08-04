// A std::function chain: handlers as callables, no class per handler.
//
// When a handler is just "test the request, and if it applies, act", a class
// with virtual methods is more ceremony than it needs. Store the chain as a
// vector of std::function<bool(const Event&)> -- each returns true if it handled
// the event -- and dispatch by trying each until one claims it. Here events are
// routed by severity: page on-call for critical, log a warning for elevated, and
// a catch-all logs the rest, so the chain always ends with a handler that says
// yes.

#include <iostream>
#include <functional>
#include <vector>
#include <string>

struct Event {
    std::string name;
    int severity;
};

int main() {
    using Handler = std::function<bool(const Event&)>; // true == "I handled it"

    std::vector<Handler> chain = {
        [](const Event& e) {
            if (e.severity >= 8) { std::cout << e.name << ": PAGE on-call\n"; return true; }
            return false;
        },
        [](const Event& e) {
            if (e.severity >= 5) { std::cout << e.name << ": log as warning\n"; return true; }
            return false;
        },
        [](const Event& e) { // catch-all terminal: always handles
            std::cout << e.name << ": log as info\n";
            return true;
        },
    };

    auto dispatch = [&chain](const Event& e) {
        for (const Handler& h : chain)
            if (h(e)) return;                       // first to claim it wins, then stop
        std::cout << e.name << ": unhandled\n";      // only if nothing claimed it
    };

    dispatch({ "disk full", 9 });
    dispatch({ "retry storm", 6 });
    dispatch({ "heartbeat", 1 });
    return 0;
}
