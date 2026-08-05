// Anti-pattern: a subtype that breaks a base invariant.
//
// Account guarantees, for its whole lifetime, that the balance never goes below
// zero. OverdraftAccount overrides withdraw to subtract any amount, so the
// balance goes negative -- an impossible state for an Account. Callers that
// trusted the floor are now wrong.
//
// The fix: a subtype must preserve the base's invariants. Model overdraft
// honestly with a visible limit instead of silently dropping the floor.

#include <iostream>

// ---------- OverdraftAccount breaks the never-negative invariant ----------
namespace bad {

class Account {
public:
    explicit Account(long balance) : balance_(balance) {}
    virtual ~Account() {}
    // invariant: balance_ >= 0 at all times.
    virtual bool withdraw(long amount) {
        if (amount > balance_) return false; // refuse: keep the invariant
        balance_ -= amount;
        return true;
    }
    long balance() const { return balance_; }
protected:
    long balance_;
};

class OverdraftAccount : public Account {
public:
    explicit OverdraftAccount(long balance) : Account(balance) {}
    // Breaks the invariant: subtracts unconditionally, balance can go negative.
    bool withdraw(long amount) override {
        balance_ -= amount;
        return true;
    }
};

} // namespace bad

// ---------- overdraft modelled with a visible, enforced limit ----------
namespace good {

class Account {
public:
    Account(long balance, long overdraftLimit)
        : balance_(balance), overdraftLimit_(overdraftLimit) {}
    virtual ~Account() {}
    // invariant: balance_ >= -overdraftLimit_ at all times, and callers can
    // read the limit, so the floor is never a surprise.
    virtual bool withdraw(long amount) {
        if (balance_ - amount < -overdraftLimit_) return false;
        balance_ -= amount;
        return true;
    }
    long balance() const { return balance_; }
    long floor() const { return -overdraftLimit_; }
protected:
    long balance_;
    long overdraftLimit_;
};

} // namespace good

int main() {
    bad::Account strict(100);
    strict.withdraw(150);            // refused: invariant holds
    std::cout << "[bad] strict balance = " << strict.balance() << "\n"; // 100

    bad::OverdraftAccount over(100);
    over.withdraw(150);              // allowed: invariant broken
    std::cout << "[bad] overdraft balance = " << over.balance() << "\n"; // -50

    good::Account acct(100, 200);    // may go to -200, and says so
    bool ok = acct.withdraw(150);
    std::cout << "[good] withdraw ok=" << (ok ? "yes" : "no")
              << " balance=" << acct.balance()
              << " floor=" << acct.floor() << "\n"; // -50, floor -200, honest
}
