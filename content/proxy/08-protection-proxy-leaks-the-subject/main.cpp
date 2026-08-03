#include <iostream>

// Subject.
struct Account {
    virtual void withdraw(int cents) = 0;
    virtual ~Account() = default;
};

// Real subject.
class RealAccount : public Account {
public:
    void withdraw(int cents) override {
        balance_ -= cents;
        std::cout << "withdrew " << cents << ", balance " << balance_ << "\n";
    }
private:
    int balance_ = 1000;
};

// ANTI-PATTERN: the protection proxy guards withdraw(), but also hands out the
// real account through unwrap(). Any caller can take the raw subject and go
// straight around the guard, so the protection is decorative -- a single leaking
// accessor undoes every check the proxy performs.
class LeakyProxy : public Account {
public:
    LeakyProxy(RealAccount& real, bool authorized) : real_(real), authorized_(authorized) {}
    void withdraw(int cents) override {
        if (!authorized_) { std::cout << "denied\n"; return; }
        real_.withdraw(cents);
    }
    RealAccount& unwrap() { return real_; }     // BUG: leaks the guarded subject
private:
    RealAccount& real_;
    bool authorized_;
};

// FIX: never expose the real subject. The proxy is the only handle a client
// gets, so every path to the subject goes through the guard.
class TightProxy : public Account {
public:
    TightProxy(RealAccount& real, bool authorized) : real_(real), authorized_(authorized) {}
    void withdraw(int cents) override {
        if (!authorized_) { std::cout << "denied\n"; return; }
        real_.withdraw(cents);
    }
private:
    RealAccount& real_;
    bool authorized_;
};

int main() {
    RealAccount acct;

    std::cout << "== leaky proxy ==\n";
    LeakyProxy leaky(acct, false);        // NOT authorized
    leaky.withdraw(100);                  // denied -- good
    leaky.unwrap().withdraw(100);         // bypass! straight to the real account

    std::cout << "== tight proxy ==\n";
    TightProxy tight(acct, false);
    tight.withdraw(100);                  // denied -- and no way around it
    return 0;
}
