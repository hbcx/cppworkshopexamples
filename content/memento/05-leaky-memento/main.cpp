// Anti-pattern: a leaky memento that exposes the originator's state.
//
// The whole point of Memento is to save state WITHOUT publishing it. A memento
// with public fields (or public getters/setters) throws that away: now anyone
// holding a snapshot can read the originator's private state, and worse, TAMPER
// with it before restoring, injecting a value the originator would never have
// produced and bypassing its invariants. The fix is the opaque memento: state
// private to the originator, which alone is a friend.

#include <iostream>

// ---- BAD: the memento's state is public ----
class AccountBad {
    int balance_;
public:
    explicit AccountBad(int b) : balance_(b) {}
    struct Memento { int balance; }; // public: readable and writable by anyone
    Memento save() const { return Memento{ balance_ }; }
    void restore(const Memento& m) { balance_ = m.balance; }
    int balance() const { return balance_; }
};

// ---- GOOD: the memento is opaque ----
class AccountGood {
    int balance_;
public:
    explicit AccountGood(int b) : balance_(b) {}
    class Memento {
        int balance_;
        explicit Memento(int b) : balance_(b) {}
        friend class AccountGood; // only the originator can read or build it
    };
    Memento save() const { return Memento(balance_); }
    void restore(const Memento& m) { balance_ = m.balance_; }
    int balance() const { return balance_; }
};

int main() {
    AccountBad bad(100);
    AccountBad::Memento snap = bad.save();
    std::cout << "peeked private balance: " << snap.balance << "\n"; // encapsulation leaked
    snap.balance = -999;   // tamper: a state the account would never produce
    bad.restore(snap);
    std::cout << "bad restored: " << bad.balance() << "\n"; // -999, invariants bypassed

    AccountGood good(100);
    AccountGood::Memento safe = good.save();
    // safe.balance_ = -999;   // would NOT compile: balance_ is private to AccountGood
    good.restore(safe);
    std::cout << "good restored: " << good.balance() << "\n"; // 100, untouchable
    return 0;
}
