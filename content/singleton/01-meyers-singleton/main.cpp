#include <iostream>
#include <map>
#include <string>

// A process-wide configuration store that must exist exactly once. The Meyers
// Singleton: instance() returns a reference to a function-local static, built
// lazily on first use and -- since C++11 -- with a thread-safe initialization.
class Config {
public:
    static Config& instance() {
        static Config inst;   // constructed once, on first call; C++11 makes this race-free
        return inst;
    }

    // Exactly one: no copies, no moves, so a second Config cannot be made.
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    void set(const std::string& key, const std::string& value) { values_[key] = value; }
    std::string get(const std::string& key) const {
        auto it = values_.find(key);
        return it == values_.end() ? std::string("<unset>") : it->second;
    }

private:
    Config() { std::cout << "Config constructed\n"; }   // private: only instance() can build it
    std::map<std::string, std::string> values_;
};

int main() {
    // First touch constructs it; the "Config constructed" line prints once.
    Config::instance().set("theme", "dark");
    Config::instance().set("retries", "3");

    // Every call hands back the same object -- compare identities, not addresses.
    bool same = &Config::instance() == &Config::instance();

    std::cout << "theme = " << Config::instance().get("theme") << '\n';
    std::cout << "same instance from two calls: " << (same ? "yes" : "no") << '\n';
    std::cout << "retries = " << Config::instance().get("retries") << '\n';
    return 0;
}
