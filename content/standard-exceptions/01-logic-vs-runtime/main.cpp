// <stdexcept> splits the standard exceptions into two families by CAUSE:
//   logic_error   -- a bug: a precondition the caller could have checked first
//                    (bad index, bad argument). Fix the code.
//   runtime_error -- a condition only knowable at run time (out of funds, disk
//                    full, bad input). Handle it; you cannot "fix" it away.
// Catching by the family base lets you react to each differently.
#include <iostream>
#include <stdexcept>

void withdraw(int balance, int amount) {
    if (amount < 0) {
        throw std::logic_error("amount must be non-negative");   // caller bug
    }
    if (amount > balance) {
        throw std::runtime_error("insufficient funds");          // runtime state
    }
    std::cout << "ok: withdrew " << amount << '\n';
}

void attempt(int balance, int amount) {
    try {
        withdraw(balance, amount);
    }
    catch (const std::logic_error& e) {
        std::cout << "bug (fix the caller): " << e.what() << '\n';
    }
    catch (const std::runtime_error& e) {
        std::cout << "runtime (handle it):  " << e.what() << '\n';
    }
}

int main() {
    attempt(100, 30);
    attempt(100, -5);
    attempt(100, 500);
    return 0;
}
