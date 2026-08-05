// Invert a dependency on time to make a time-dependent policy testable.
//
// TokenExpiry asks: given when a token was issued and a time-to-live, is it
// expired NOW? If it read the system clock directly, the boundary cases would be
// almost impossible to test. Depending on a Clock abstraction lets a test inject
// a FakeClock and pin now(), so every case is deterministic.

#include <iostream>
#include <string>

// The abstraction the policy depends on instead of the system clock.
class Clock {
public:
    virtual ~Clock() {}
    virtual long now() const = 0; // seconds since some epoch
};

// High-level policy: pure logic over now(), issue time, and a TTL.
class TokenExpiry {
public:
    TokenExpiry(const Clock& clock, long ttl) : clock_(clock), ttl_(ttl) {}
    bool expired(long issuedAt) const {
        return clock_.now() - issuedAt >= ttl_;
    }
private:
    const Clock& clock_;
    long ttl_;
};

// A test double that makes the policy deterministic. In production a real
// adapter would read the system clock behind this same interface.
class FakeClock : public Clock {
public:
    explicit FakeClock(long t) : t_(t) {}
    long now() const override { return t_; }
    void set(long t) { t_ = t; }
private:
    long t_;
};

static void check(const char* label, bool value) {
    std::cout << label << ": " << (value ? "expired" : "valid") << "\n";
}

int main() {
    FakeClock clock(1000);
    TokenExpiry expiry(clock, 30); // 30-second TTL

    // Token issued at t=980: 1000 - 980 = 20 < 30 -> still valid.
    check("issued at 980, now 1000", expiry.expired(980));

    // Advance the clock past the boundary with no real waiting.
    clock.set(1015); // 1015 - 980 = 35 >= 30 -> expired
    check("issued at 980, now 1015", expiry.expired(980));

    // Exact boundary is reachable too: 1010 - 980 = 30 >= 30 -> expired.
    clock.set(1010);
    check("issued at 980, now 1010", expiry.expired(980));
}
