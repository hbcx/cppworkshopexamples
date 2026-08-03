#include <iostream>
#include <map>
#include <string>

// Subject.
struct Config {
    virtual int get(const std::string& key) = 0;
    virtual ~Config() = default;
};

// Real subject: a live store whose values change over time.
class LiveConfig : public Config {
public:
    void set(const std::string& k, int v) { data_[k] = v; }
    int get(const std::string& k) override { return data_[k]; }
private:
    std::map<std::string, int> data_;
};

// ANTI-PATTERN: a caching proxy that caches a MUTABLE value forever. A proxy must
// be transparent -- behave like the subject a client cannot distinguish it from.
// This one keeps returning the first value it saw even after the real config
// changed, so proxy and subject now disagree, and the client gets stale data.
class StaleProxy : public Config {
public:
    explicit StaleProxy(LiveConfig& real) : real_(real) {}
    int get(const std::string& k) override {
        auto it = cache_.find(k);
        if (it != cache_.end()) return it->second;   // never invalidated -- BUG
        int v = real_.get(k);
        cache_[k] = v;
        return v;
    }
private:
    LiveConfig& real_;
    std::map<std::string, int> cache_;
};

// FIX: a transparent proxy preserves the observable behaviour. If the value can
// change, do not cache it (or invalidate on change). This proxy forwards, so it
// always agrees with the subject.
class ForwardingProxy : public Config {
public:
    explicit ForwardingProxy(LiveConfig& real) : real_(real) {}
    int get(const std::string& k) override { return real_.get(k); }
private:
    LiveConfig& real_;
};

int main() {
    LiveConfig cfg;
    cfg.set("timeout", 30);

    StaleProxy stale(cfg);
    std::cout << "stale proxy first read: " << stale.get("timeout") << "\n";   // 30
    cfg.set("timeout", 60);                                                    // value changes
    std::cout << "real value now:         " << cfg.get("timeout") << "\n";     // 60
    std::cout << "stale proxy still says: " << stale.get("timeout") << "\n";   // 30 -- wrong

    ForwardingProxy good(cfg);
    std::cout << "transparent proxy says: " << good.get("timeout") << "\n";    // 60 -- agrees
    return 0;
}
