#include <functional>
#include <iostream>
#include <string>

struct Account {
    std::string owner;
    long balance;

    void deposit(long amount) { balance += amount; }
    long report() const { return balance; }
};

int main() {
    Account acc{"Ada", 100};

    // A member function needs an object to be called ON. The simplest, clearest
    // way to store one in a std::function is a lambda that captures the object
    // (here by pointer) and calls the member.
    std::function<void(long)> deposit = [&acc](long amt) { acc.deposit(amt); };
    deposit(50);
    std::cout << "after deposit: " << acc.report() << "\n";   // 150

    // std::function can also wrap a pointer-to-member directly: the signature
    // then takes the object as its first argument. std::mem_fn does the same and
    // reads better (both are covered in the invoke/bind/mem_fn section).
    std::function<void(Account&, long)> memDeposit = &Account::deposit;
    memDeposit(acc, 25);
    std::cout << "after memDeposit: " << acc.report() << "\n"; // 175
    return 0;
}
