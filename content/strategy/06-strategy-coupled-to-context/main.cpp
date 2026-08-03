#include <iostream>
#include <string>
#include <utility>

class Order;   // forward declaration

// ANTI-PATTERN: the strategy takes a reference to the whole Order and reaches into
// its internals. Now the strategy is coupled to the context's structure -- it
// breaks when Order changes, cannot be reused with any other context, and cannot
// be unit-tested without building a full Order. The two can no longer vary apart.
struct ShippingBad {
    virtual int cost(const Order& order) const = 0;   // knows the entire context
    virtual ~ShippingBad() = default;
};

class Order {
public:
    Order(int weight, int distance, std::string customer)
        : weight_(weight), distance_(distance), customer_(std::move(customer)) {}
    int weight() const { return weight_; }
    int distance() const { return distance_; }
    const std::string& customer() const { return customer_; }
private:
    int weight_;
    int distance_;
    std::string customer_;
};

struct ByWeightBad : ShippingBad {
    int cost(const Order& order) const override { return order.weight() * 2; }   // uses one field
};

// FIX: the strategy takes only the data the algorithm needs. It no longer knows
// Order at all, so it is reusable anywhere, testable in isolation with plain
// ints, and unaffected by changes to the context's shape.
struct Shipping {
    virtual int cost(int weight, int distance) const = 0;
    virtual ~Shipping() = default;
};

struct ByWeight : Shipping {
    int cost(int weight, int /*distance*/) const override { return weight * 2; }
};

int main() {
    Order order(5, 100, "alice");

    std::cout << "== strategy coupled to the whole context ==\n";
    ByWeightBad bad;
    std::cout << "cost: " << bad.cost(order) << "\n";   // 10

    std::cout << "== strategy takes only what it needs ==\n";
    ByWeight good;
    std::cout << "cost: " << good.cost(order.weight(), order.distance()) << "\n";   // 10
    return 0;
}
