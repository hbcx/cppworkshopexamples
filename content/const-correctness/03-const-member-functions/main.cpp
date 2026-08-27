#include <iostream>

// A const member function promises not to modify the object it is called on.
// Inside it, this is a pointer to const, so the function may read members but
// not write them. Only const member functions can be called on a const object
// or through a const reference -- so marking your read-only methods const is
// what makes your type usable in const contexts.

class Account {
public:
    explicit Account(long cents) : balance_(cents) {}

    // Read-only: marked const, so callable on const Accounts.
    long balance() const { return balance_; }
    bool isOverdrawn() const { return balance_ < 0; }

    // Modifies the object: NOT const.
    void deposit(long cents) { balance_ += cents; }

private:
    long balance_;
};

// Takes the account by const reference -- it can call only the const methods.
void report(const Account& acc) {
    std::cout << "balance " << acc.balance()
              << (acc.isOverdrawn() ? " (overdrawn)" : "") << "\n";
    // acc.deposit(100);   // error: deposit is not const
}

int main() {
    Account acc(500);
    report(acc);

    acc.deposit(-800);      // a non-const object can call non-const methods
    report(acc);            // now overdrawn

    const Account frozen(1000);
    std::cout << "frozen balance = " << frozen.balance() << "\n";  // const method OK
    // frozen.deposit(1);   // error: cannot call a non-const method on a const object
    return 0;
}
