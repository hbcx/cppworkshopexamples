#include <iostream>
#include <memory>

struct Config {
    int port;
    explicit Config(int p) : port(p) {}
};

// A function that only READS a Config which is guaranteed to outlive the call.
// It does not need to observe a maybe-dead object.

// Anti-pattern: taking a weak_ptr here. It forces the Config to be shared_ptr-
// owned just so a weak_ptr can exist, adds atomic weak-count traffic, and makes
// every use a lock() plus a null check for a pointer that is never really null:
//     int read_bad(const std::weak_ptr<Config>& w) {
//         auto c = w.lock();          // pointless: c is always valid here
//         return c ? c->port : -1;
//     }

// The fix: a plain reference (or raw pointer) says "I observe, I do not own, and
// I rely on you to keep it alive for my call" -- no shared ownership, no atomics,
// no lock.
static int read(const Config& c) { return c.port; }

int main() {
    Config cfg(8080);                 // a plain stack object -- not shared at all
    std::cout << "port = " << read(cfg) << '\n';

    // read() also works on a heap Config without forcing shared_ptr on callers:
    std::unique_ptr<Config> owned(new Config(9090));
    std::cout << "port = " << read(*owned) << '\n';

    std::cout << "no control block, no weak count, no lock needed\n";
    return 0;
}
