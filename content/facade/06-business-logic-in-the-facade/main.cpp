#include <iostream>
#include <string>

// Subsystem: the single source of truth for pricing rules.
class PricingService {
public:
    // The real discount policy lives here: 15% off for bulk (qty >= 10).
    int priceFor(int unitCents, int qty) const {
        const int gross = unitCents * qty;
        return (qty >= 10) ? gross - gross * 15 / 100 : gross;
    }
};

class Ledger {
public:
    void record(int cents) { std::cout << "ledger: recording " << cents << " cents\n"; }
};

// ANTI-PATTERN: the facade computes the discount ITSELF instead of asking the
// pricing subsystem. The bulk rule now lives in two places -- here and in
// PricingService -- and they have already drifted: this hardcodes 10%, the real
// policy is 15%. A facade should orchestrate, not re-decide business rules.
class OrderFacadeBad {
public:
    int total(int unitCents, int qty) {
        const int gross = unitCents * qty;
        const int net = (qty >= 10) ? gross - gross * 10 / 100 : gross;   // duplicated, and wrong
        ledger_.record(net);
        return net;
    }
private:
    Ledger ledger_;
};

// FIX: the facade delegates the rule to the subsystem that owns it and only
// orchestrates -- price, then record. There is one home for the policy.
class OrderFacadeGood {
public:
    int total(int unitCents, int qty) {
        const int net = pricing_.priceFor(unitCents, qty);
        ledger_.record(net);
        return net;
    }
private:
    PricingService pricing_;
    Ledger ledger_;
};

int main() {
    const int unit = 100;   // cents per unit
    const int qty = 10;

    OrderFacadeBad bad;
    std::cout << "fat facade total:  " << bad.total(unit, qty) << " cents\n";    // 900 (wrong, 10%)

    OrderFacadeGood good;
    std::cout << "thin facade total: " << good.total(unit, qty) << " cents\n";   // 850 (correct, 15%)
    return 0;
}
