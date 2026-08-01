#include <iostream>
#include <string>

// Two services that need each other: Orders asks Inventory to reserve, and
// Inventory calls back into Orders when stock changes. That mutual need is a
// dependency cycle.
//
// Anti-pattern (will not construct): if BOTH take the other in the constructor
//     Orders(Inventory& inv);  Inventory(Orders& ord);
// you cannot build either first -- each constructor demands an already-built
// instance of the other, so no valid construction order exists. Shown only as a
// comment, because it genuinely does not compile into a buildable graph.

class Orders;

// Fix: break the cycle by injecting one side after construction (setter
// injection). Inventory is built without Orders, so it can go first.
class Inventory {
public:
    void setOrders(Orders* orders) { orders_ = orders; }   // wired in later
    void restock(const std::string& sku);
    bool reserve(const std::string& sku) {
        std::cout << "  reserved " << sku << '\n';
        return true;
    }
private:
    Orders* orders_ = nullptr;   // non-owning, filled in once Orders exists
};

class Orders {
public:
    explicit Orders(Inventory& inv) : inv_(inv) {}
    void place(const std::string& sku) { inv_.reserve(sku); }
    void onRestock(const std::string& sku) {
        std::cout << "  order notified of restock: " << sku << '\n';
    }
private:
    Inventory& inv_;
};

void Inventory::restock(const std::string& sku) {
    std::cout << "  restocked " << sku << '\n';
    if (orders_) orders_->onRestock(sku);
}

int main() {
    // A valid construction order now exists:
    Inventory inventory;            // 1. build the side that can stand alone
    Orders orders(inventory);       // 2. build the side that needs it
    inventory.setOrders(&orders);   // 3. close the loop with setter injection

    orders.place("widget");
    inventory.restock("widget");
    return 0;
}
