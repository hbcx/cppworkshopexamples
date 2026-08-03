#include <iostream>
#include <string>

class Inventory {
public:
    bool reserve(int qty) {
        std::cout << "inventory: reserve " << qty << "\n";
        return qty <= stock_;
    }
    void release(int qty) { std::cout << "inventory: release " << qty << "\n"; }
private:
    int stock_ = 1;                 // only one unit in stock
};

class Payment {
public:
    void charge(int cents) { std::cout << "payment: charge " << cents << " cents\n"; }
    void refund(int cents) { std::cout << "payment: refund " << cents << " cents\n"; }
};

// The facade centralises the correct order and the rollback: reserve first,
// charge only if the reservation succeeded, and never leave money taken without
// stock behind it.
class OrderFacade {
public:
    bool place(int qty, int cents) {
        if (!inventory_.reserve(qty)) {
            std::cout << "facade: out of stock, nothing charged\n";
            return false;
        }
        payment_.charge(cents);
        std::cout << "facade: order placed\n";
        return true;
    }
private:
    Inventory inventory_;
    Payment payment_;
};

int main() {
    const int qty = 5;        // more than the single unit in stock
    const int cents = 1800;

    // ANTI-PATTERN: the client bypasses the facade and orchestrates the subsystem
    // itself -- and gets the sequence wrong. It charges first, then discovers
    // there is no stock, and this hand-rolled copy of the workflow forgot the
    // rollback, leaving the customer charged with nothing reserved.
    std::cout << "== client bypasses the facade ==\n";
    Inventory inv;
    Payment pay;
    pay.charge(cents);                          // charged up front
    if (!inv.reserve(qty)) {
        std::cout << "bypass: out of stock -- but already charged, no refund issued!\n";
    }

    // FIX: route the common path through the facade, which does it correctly --
    // it reserves first and never charges when the stock is not there.
    std::cout << "== through the facade ==\n";
    OrderFacade facade;
    facade.place(qty, cents);
    return 0;
}
