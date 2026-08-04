#include <iostream>
#include <memory>

struct Config {
    int port;
    explicit Config(int p) : port(p) {}
};

// Anti-pattern: a shared_ptr that is never actually shared. If the object has a
// single owner for its whole life, shared_ptr pays for a heap-allocated control
// block and atomic reference counting that buy nothing -- and it hides that the
// ownership is really unique. The sign is a use_count() that is always 1.
static void useShared() {
    std::shared_ptr<Config> cfg = std::make_shared<Config>(8080);
    // ...used only here, only observed, never copied to share...
    std::cout << "[shared] port " << cfg->port
              << ", use_count stays " << cfg.use_count() << '\n';   // always 1
}

// The fix: unique_ptr says exactly what is true -- one owner -- with no control
// block and no atomics. Widen to shared_ptr only if real sharing appears later.
static void useUnique() {
    std::unique_ptr<Config> cfg(new Config(8080));
    std::cout << "[unique] port " << cfg->port << ", single owner, no refcount\n";
}

int main() {
    useShared();
    useUnique();
    std::cout << "same behaviour; unique_ptr is cheaper and states the intent\n";
    return 0;
}
