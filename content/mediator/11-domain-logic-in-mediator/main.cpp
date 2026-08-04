// Anti-pattern: domain logic in the mediator, empty colleagues.
//
// A mediator should coordinate INTERACTIONS, not own the colleagues' own rules.
// When a colleague's business logic -- here an account's overdraft rule -- is
// lifted into the mediator, the colleague becomes empty (just getters and
// setters) and the rule ends up somewhere it cannot be reused or enforced
// independently. The fix is to keep each colleague's rules inside the colleague
// and let the mediator only coordinate: on a successful withdrawal, record it.
// The output is identical; the placement of responsibility is not.

#include <iostream>

class Ledger {
public:
    void record(int amount) { std::cout << "  ledger: recorded -" << amount << "\n"; }
};

// ---- BAD: the mediator enforces the account's overdraft rule ----
class AccountAnemic {
    int balance_;
public:
    explicit AccountAnemic(int b) : balance_(b) {}
    int balance() const { return balance_; }
    void setBalance(int b) { balance_ = b; } // no rules of its own
};
class BankMediatorBad {
    AccountAnemic& account_;
    Ledger& ledger_;
public:
    BankMediatorBad(AccountAnemic& a, Ledger& l) : account_(a), ledger_(l) {}
    void withdraw(int amount) {
        // the OVERDRAFT RULE lives here, in the mediator, not in the account:
        if (account_.balance() < amount) { std::cout << "  denied: insufficient funds\n"; return; }
        account_.setBalance(account_.balance() - amount);
        ledger_.record(amount);
    }
};

// ---- GOOD: the account owns its rule; the mediator only coordinates ----
class Account {
    int balance_;
public:
    explicit Account(int b) : balance_(b) {}
    int balance() const { return balance_; }
    bool withdraw(int amount) { // the rule lives WITH the data it guards
        if (balance_ < amount) return false;
        balance_ -= amount;
        return true;
    }
};
class BankMediatorGood {
    Account& account_;
    Ledger& ledger_;
public:
    BankMediatorGood(Account& a, Ledger& l) : account_(a), ledger_(l) {}
    void withdraw(int amount) {
        // coordinate only: ask the account, and on success tell the ledger.
        if (!account_.withdraw(amount)) { std::cout << "  denied: insufficient funds\n"; return; }
        ledger_.record(amount);
    }
};

int main() {
    std::cout << "-- bad (rule in mediator) --\n";
    AccountAnemic a1(100);
    Ledger l1;
    BankMediatorBad bad(a1, l1);
    bad.withdraw(30);  // ok
    bad.withdraw(200); // denied

    std::cout << "-- good (rule in account) --\n";
    Account a2(100);
    Ledger l2;
    BankMediatorGood good(a2, l2);
    good.withdraw(30);  // ok
    good.withdraw(200); // denied
    return 0;
}
