// Anti-pattern: the god class.
//
// OrderProcessor does five unrelated jobs -- validate, price, format a receipt,
// persist, and log. Five responsibilities, five reasons to change, one class
// that every team has to edit. It compiles and runs; the cost is that nothing
// in it can change or be tested in isolation.
//
// The fix splits each concern into its own small class over a plain Order.

#include <iostream>
#include <string>
#include <vector>

struct Item {
    std::string name;
    long priceCents;
    int qty;
};

struct Order {
    std::string customer;
    std::vector<Item> items;
};

// ---------- the god class ----------
namespace bad {

class OrderProcessor {
public:
    void process(const Order& o) {
        // 1) validation
        if (o.customer.empty() || o.items.empty()) {
            std::cout << "[bad] rejected: empty order\n";
            return;
        }
        // 2) pricing
        long total = 0;
        for (const Item& it : o.items) total += it.priceCents * it.qty;
        // 3) receipt formatting
        std::cout << "[bad] receipt for " << o.customer << ": $"
                  << total / 100 << "." << (total % 100 < 10 ? "0" : "")
                  << total % 100 << "\n";
        // 4) persistence
        std::cout << "[bad] db: insert order for " << o.customer << "\n";
        // 5) logging
        std::cout << "[bad] log: processed " << o.items.size() << " items\n";
    }
};

} // namespace bad

// ---------- one class per responsibility ----------
namespace good {

class OrderValidator {
public:
    bool isValid(const Order& o) const {
        return !o.customer.empty() && !o.items.empty();
    }
};

class PriceCalculator {
public:
    long total(const Order& o) const {
        long sum = 0;
        for (const Item& it : o.items) sum += it.priceCents * it.qty;
        return sum;
    }
};

class ReceiptFormatter {
public:
    std::string format(const std::string& customer, long cents) const {
        return "receipt for " + customer + ": $" + std::to_string(cents / 100) +
               "." + (cents % 100 < 10 ? "0" : "") + std::to_string(cents % 100);
    }
};

class OrderRepository {
public:
    void save(const Order& o) const {
        std::cout << "[good] db: insert order for " << o.customer << "\n";
    }
};

} // namespace good

int main() {
    Order o{"Acme Corp", {{"Widget", 999, 3}, {"Gadget", 1995, 1}}};

    bad::OrderProcessor processor;
    processor.process(o);

    std::cout << "---\n";

    // Each concern is now testable and changeable on its own.
    good::OrderValidator validator;
    good::PriceCalculator pricing;
    good::ReceiptFormatter formatter;
    good::OrderRepository repo;

    if (validator.isValid(o)) {
        long total = pricing.total(o);
        std::cout << "[good] " << formatter.format(o.customer, total) << "\n";
        repo.save(o);
    }
}
