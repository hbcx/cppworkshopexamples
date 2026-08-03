#include <iostream>

// Strategy interface: one operation, the algorithm that varies. Concrete
// strategies each implement it; the context depends only on this.
struct PricingStrategy {
    virtual int priceFor(int base) const = 0;
    virtual ~PricingStrategy() = default;
};

class RegularPricing : public PricingStrategy {
public:
    int priceFor(int base) const override { return base; }
};

class MemberPricing : public PricingStrategy {
public:
    int priceFor(int base) const override { return base - base / 10; }   // 10% off
};

class ClearancePricing : public PricingStrategy {
public:
    int priceFor(int base) const override { return base / 2; }           // 50% off
};

// Context: holds a strategy and delegates the varying step to it. Its own logic
// (checkout) stays the same; only the injected strategy changes the result, and
// it can be swapped at runtime.
class Checkout {
public:
    explicit Checkout(const PricingStrategy& p) : pricing_(&p) {}
    void setPricing(const PricingStrategy& p) { pricing_ = &p; }
    int total(int base) const { return pricing_->priceFor(base); }
private:
    const PricingStrategy* pricing_;
};

int main() {
    RegularPricing regular;
    MemberPricing member;
    ClearancePricing clearance;

    Checkout cart(regular);
    std::cout << "regular:   " << cart.total(100) << "\n";   // 100

    cart.setPricing(member);
    std::cout << "member:    " << cart.total(100) << "\n";   // 90

    cart.setPricing(clearance);
    std::cout << "clearance: " << cart.total(100) << "\n";   // 50
    return 0;
}
