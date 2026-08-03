#include <iostream>
#include <string>

// Subsystem: three services, each with its own responsibility. Getting an order
// right means calling them in the correct order and honouring the dependencies
// between them -- reserve stock before charging, ship only after payment, and
// undo the reservation if payment fails.
class Inventory {
public:
    bool reserve(const std::string& sku, int qty) {
        std::cout << "inventory: reserving " << qty << " x " << sku << '\n';
        return qty <= stock_;
    }
    void release(const std::string& sku, int qty) {
        std::cout << "inventory: releasing " << qty << " x " << sku << '\n';
    }
private:
    int stock_ = 10;
};

class Payment {
public:
    bool charge(const std::string& account, int cents) {
        std::cout << "payment: charging " << cents << " cents to " << account << '\n';
        return cents <= limit_;
    }
private:
    int limit_ = 5000;
};

class Shipping {
public:
    void schedule(const std::string& sku, int qty) {
        std::cout << "shipping: scheduling delivery of " << qty << " x " << sku << '\n';
    }
};

// Facade: the client wants to place an order. It should not have to know the
// three services, their call order, or the rollback rule between them.
// placeOrder encodes that whole dance once and returns a simple yes/no.
class Checkout {
public:
    bool placeOrder(const std::string& sku, int qty, const std::string& account, int cents) {
        if (!inventory_.reserve(sku, qty)) {
            std::cout << "checkout: out of stock\n";
            return false;
        }
        if (!payment_.charge(account, cents)) {
            std::cout << "checkout: payment declined, rolling back\n";
            inventory_.release(sku, qty);          // undo the reservation
            return false;
        }
        shipping_.schedule(sku, qty);
        std::cout << "checkout: order placed\n";
        return true;
    }
private:
    Inventory inventory_;
    Payment payment_;
    Shipping shipping_;
};

int main() {
    Checkout checkout;

    std::cout << "== order 1 ==\n";
    checkout.placeOrder("BOOK-42", 2, "alice", 1800);    // succeeds

    std::cout << "== order 2 ==\n";
    checkout.placeOrder("BOOK-42", 2, "bob", 9000);      // payment declined -> rollback
    return 0;
}
