// Anti-pattern: the god mediator.
//
// Mediator centralises interaction, and its failure mode is centralising too
// much: the mediator swells into a single class that knows every subsystem's
// rules, so every feature touches it, it cannot be understood in one sitting,
// and it becomes the bottleneck the pattern was supposed to remove. HomeHubGod
// below mixes lighting, security and climate rules in one method. The fix is to
// split by cohesive concern into focused mediators; the behaviour is identical,
// the structure is not.

#include <iostream>
#include <string>

// ---- BAD: one mediator that owns every subsystem's rules ----
class HomeHubGod {
public:
    void event(const std::string& e) {
        // lighting
        if (e == "motion") std::cout << "  lights on\n";
        if (e == "dark")   std::cout << "  lights dim\n";
        // security
        if (e == "motion") std::cout << "  log motion\n";
        if (e == "smoke")  std::cout << "  ALARM\n";
        // climate
        if (e == "smoke")  std::cout << "  hvac off\n";
        if (e == "cold")   std::cout << "  heater on\n";
        // every new subsystem or rule edits THIS one method
    }
};

// ---- GOOD: one focused mediator per cohesive concern ----
struct Lighting {
    void event(const std::string& e) {
        if (e == "motion") std::cout << "  lights on\n";
        if (e == "dark")   std::cout << "  lights dim\n";
    }
};
struct Security {
    void event(const std::string& e) {
        if (e == "motion") std::cout << "  log motion\n";
        if (e == "smoke")  std::cout << "  ALARM\n";
    }
};
struct Climate {
    void event(const std::string& e) {
        if (e == "smoke") std::cout << "  hvac off\n";
        if (e == "cold")  std::cout << "  heater on\n";
    }
};

int main() {
    const std::string events[] = { "motion", "smoke", "cold" };

    std::cout << "-- god mediator --\n";
    HomeHubGod god;
    for (const std::string& e : events) { std::cout << e << ":\n"; god.event(e); }

    std::cout << "-- split mediators --\n";
    Lighting lighting; Security security; Climate climate;
    for (const std::string& e : events) {
        std::cout << e << ":\n";
        lighting.event(e); // same order -> same output, but each concern is separate
        security.event(e);
        climate.event(e);
    }
    return 0;
}
