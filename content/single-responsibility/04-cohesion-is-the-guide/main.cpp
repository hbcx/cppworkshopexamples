// SRP is about REASONS TO CHANGE, not method count.
//
// Money below has many methods -- and it is not a god class. Every method
// serves one idea: money arithmetic. They all change for the same reason, so
// the class is highly cohesive and correct as one unit.
//
// The real violation is low cohesion at ANY size: the sketched Connection has
// only three methods but three unrelated owners (network, protocol, pricing).
// Count actors, not lines.

#include <iostream>
#include <string>

// One responsibility -- represent an amount of money -- expressed through many
// cohesive methods. Splitting this would help no one.
class Money {
public:
    Money() : cents_(0) {}
    explicit Money(long cents) : cents_(cents) {}

    static Money fromDollars(long d, int c) { return Money(d * 100 + c); }

    Money operator+(Money o) const { return Money(cents_ + o.cents_); }
    Money operator-(Money o) const { return Money(cents_ - o.cents_); }
    Money scaledBy(int factor) const { return Money(cents_ * factor); }
    Money percent(int pct) const { return Money(cents_ * pct / 100); }

    bool operator<(Money o) const { return cents_ < o.cents_; }
    bool isZero() const { return cents_ == 0; }
    long cents() const { return cents_; }

    std::string format() const {
        long c = cents_ < 0 ? -cents_ : cents_;
        std::string frac = std::to_string(c % 100);
        if (frac.size() < 2) frac = "0" + frac;
        return std::string(cents_ < 0 ? "-$" : "$") +
               std::to_string(c / 100) + "." + frac;
    }

private:
    long cents_; // all the methods above turn on this one field: high cohesion
};

// Sketch of the OPPOSITE: only three methods, but three unrelated actors.
// (Shown, not run -- it is here to contrast, and would really be three classes.)
class Connection {
public:
    void openSocket();          // owned by the network team
    std::string parseRequest(); // owned by the protocol team
    Money priceOrder();         // owned by the pricing team
};

int main() {
    Money unit = Money::fromDollars(19, 95);
    Money order = unit.scaledBy(3);              // 3 units
    Money tax = order.percent(8);                // 8% tax
    Money total = order + tax;

    std::cout << "unit:  " << unit.format()  << "\n";
    std::cout << "order: " << order.format() << "\n";
    std::cout << "tax:   " << tax.format()   << "\n";
    std::cout << "total: " << total.format() << "\n";
    std::cout << "free?  " << (total.isZero() ? "yes" : "no") << "\n";
    std::cout << "Money has many methods and ONE reason to change.\n";
}
