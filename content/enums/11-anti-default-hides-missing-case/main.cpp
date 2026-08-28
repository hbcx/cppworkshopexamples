#include <iostream>

// Anti-pattern: adding a catch-all default to a switch over an enum. The default
// looks tidy, but it turns off the compiler's best help: with -Wall the compiler
// warns when a switch does not cover every enumerator (-Wswitch) -- but ONLY if
// there is no default. A default silences that, so a case you never handled (or
// one added later) is quietly routed to default instead of failing the build.

enum class Event { Connect, Disconnect, Timeout };

// BAD: the default hides the un-handled Timeout case. This compiles with no
// warning and treats Timeout as "other" -- a real event, silently swallowed.
const char* handleBad(Event e) {
    switch (e) {
        case Event::Connect:    return "connect";
        case Event::Disconnect: return "disconnect";
        default:                return "other";      // swallows Timeout
    }
}

// GOOD: no default. Every case is listed, so -Wswitch would flag the day a new
// enumerator is added and this switch stops being exhaustive.
const char* handleGood(Event e) {
    switch (e) {
        case Event::Connect:    return "connect";
        case Event::Disconnect: return "disconnect";
        case Event::Timeout:    return "timeout";
    }
    return "unreachable";   // only for a value cast in from outside the enum
}

int main() {
    Event e = Event::Timeout;
    std::cout << "bad  handler: " << handleBad(e)  << "\n";   // "other" -- lost!
    std::cout << "good handler: " << handleGood(e) << "\n";   // "timeout"
    return 0;
}
