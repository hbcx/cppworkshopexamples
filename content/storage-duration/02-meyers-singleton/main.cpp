#include <iostream>

struct Config {
    int version;
    Config() : version(3) { std::cout << "  Config constructed (once, on first use)\n"; }
};

// The Meyers singleton: a function returning a reference to a function-local
// static. The instance is created lazily on the first call, and since C++11 that
// initialization is thread-safe -- two threads racing to the first call cannot
// construct it twice.
Config& config() {
    static Config instance;
    return instance;
}

int main() {
    std::cout << "before first use\n";
    std::cout << "version = " << config().version << "\n";   // constructs here
    std::cout << "same instance across calls? " << std::boolalpha
              << (&config() == &config()) << "\n";
    return 0;
}
