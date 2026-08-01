#include <iostream>
#include <memory>
#include <string>

// A dependency several consumers share: one clock, injected into each. A fake
// monotonic tick keeps the example deterministic.
struct Clock {
    virtual int now() = 0;
    virtual ~Clock() = default;
};

struct FakeClock : Clock {
    int t = 0;
    int now() override { return ++t; }
};

// An optional dependency: a metrics sink the service runs fine without.
struct Metrics {
    virtual void count(const std::string& name) = 0;
    virtual ~Metrics() = default;
};

struct ConsoleMetrics : Metrics {
    void count(const std::string& name) override {
        std::cout << "  metric: " << name << '\n';
    }
};

class Service {
public:
    // Constructor injection for the REQUIRED dependency: the service cannot
    // exist without a clock, so it belongs in the constructor. shared_ptr
    // because the clock is genuinely shared with other services.
    explicit Service(std::shared_ptr<Clock> clock) : clock_(std::move(clock)) {}

    // Setter injection for an OPTIONAL dependency: absent by default, wired in
    // only when wanted. Use sparingly -- it permits a half-configured object.
    void setMetrics(Metrics* metrics) { metrics_ = metrics; }

    void handle(const std::string& req) {
        std::cout << "t=" << clock_->now() << " handled " << req << '\n';
        if (metrics_) metrics_->count("handled");
    }
private:
    std::shared_ptr<Clock> clock_;   // shared ownership: many services, one clock
    Metrics* metrics_ = nullptr;     // non-owning, optional
};

int main() {
    auto clock = std::make_shared<FakeClock>();

    // Two services share one clock; the ticks are continuous across both,
    // proving they hold the same instance.
    Service a(clock);
    Service b(clock);

    ConsoleMetrics metrics;
    a.setMetrics(&metrics);   // optional dependency, only on service a

    a.handle("GET /");        // t=1, emits a metric
    b.handle("GET /health");  // t=2, no metric
    a.handle("POST /login");  // t=3, emits a metric

    std::cout << "clock owners: " << clock.use_count() << " (a, b, and the local)\n";
    return 0;
}
