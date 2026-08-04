#include <iostream>

// ANTI-PATTERN: the command IMPLEMENTS the work itself instead of delegating to a
// receiver. It now owns account logic, so the same rule is duplicated here and in
// the real account, the command cannot be reused or tested without carrying a copy
// of the domain, and it is coupled to everything it manipulates.
class DepositCommandBad {
public:
    DepositCommandBad(int& balance, int amount) : balance_(balance), amount_(amount) {}
    void execute() {
        if (amount_ <= 0) { std::cout << "invalid amount\n"; return; }   // logic in the command -- BUG
        balance_ += amount_;
        std::cout << "balance now " << balance_ << "\n";
    }
private:
    int& balance_;
    int amount_;
};

// FIX: the receiver owns the logic; the command just binds it and forwards. The
// rule has one home, and the command is a thin, reusable request.
class Account {
public:
    void deposit(int amount) {
        if (amount <= 0) { std::cout << "invalid amount\n"; return; }
        balance_ += amount;
        std::cout << "balance now " << balance_ << "\n";
    }
private:
    int balance_ = 0;
};

class DepositCommandGood {
public:
    DepositCommandGood(Account& a, int amount) : account_(a), amount_(amount) {}
    void execute() { account_.deposit(amount_); }   // delegate to the receiver
private:
    Account& account_;
    int amount_;
};

int main() {
    std::cout << "== fat command (logic inside the command) ==\n";
    int balance = 0;
    DepositCommandBad(balance, 100).execute();
    DepositCommandBad(balance, -5).execute();

    std::cout << "== thin command delegating to a receiver ==\n";
    Account account;
    DepositCommandGood(account, 100).execute();
    DepositCommandGood(account, -5).execute();
    return 0;
}
