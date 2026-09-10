#include <iostream>

// A separate flag with static storage, constant-initialized, so it is valid for
// the whole program -- safe to read even after the Logger object is destroyed.
bool g_loggerAlive = false;

struct Logger {
    Logger()  { g_loggerAlive = true; }
    ~Logger() { g_loggerAlive = false; }
    void log(const char* m) const { std::cout << "  [log] " << m << "\n"; }
};

extern Logger g_logger;   // defined below, AFTER g_service

// ANTI-PATTERN: the destruction-order version of the fiasco. A static object whose
// destructor uses another static. Statics are destroyed in reverse construction
// order; g_service is defined -- so constructed -- before g_logger, which means it
// is destroyed AFTER g_logger. Its destructor then finds the logger already gone.
// Across translation units the order is unspecified, so this can strike code that
// looks correctly ordered.
struct Service {
    ~Service() {
        if (g_loggerAlive)
            g_logger.log("Service shutting down");
        else
            std::cout << "  Service dtor: logger already destroyed -- message lost\n";
    }
};

Service g_service;   // constructed FIRST  -> destroyed LAST
Logger  g_logger;    // constructed SECOND -> destroyed FIRST

// FIX (described): give the logger a lifetime that outlives its users -- a Meyers
// singleton, or a deliberately leaked object that is never destroyed:
//     Logger& logger() { static Logger* p = new Logger(); return *p; }
// so it is still available during every other static's destruction. Guarding on a
// separate still-alive flag before touching the object, as done above, is the
// defensive minimum when you cannot control the order.

int main() {
    std::cout << "main running; statics are destroyed at exit\n";
    g_logger.log("hello from main");
    return 0;
}
