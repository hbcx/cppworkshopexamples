#include <iostream>
#include <new>
#include <string>
#include <utility>

struct Logger {
    static int open;
    std::string name;
    explicit Logger(std::string n) : name(std::move(n)) { ++open; }
    ~Logger() { --open; }        // real cleanup lives here
};
int Logger::open = 0;

int main() {
    alignas(Logger) unsigned char buf1[sizeof(Logger)];
    alignas(Logger) unsigned char buf2[sizeof(Logger)];

    // BAD: construct with placement new, then let the buffer go away WITHOUT
    // calling the destructor. Placement new has no matching delete, so
    // ~Logger never runs -- its cleanup (and any owned resource) is skipped.
    new (buf1) Logger("leaked");
    std::cout << "after placement new, no destroy: open = "
              << Logger::open << "\n";

    Logger::open = 0;            // reset so the clean demo stands alone

    // GOOD: every placement new is paired with an explicit destructor call.
    Logger* g = new (buf2) Logger("released");
    g->~Logger();
    std::cout << "after explicit destructor:       open = "
              << Logger::open << "\n";
}
