#include <iostream>
#include <memory>
#include <utility>

// ANTI-PATTERN: a Strategy interface introduced for a single algorithm that has
// no second variant and no prospect of one. The interface, the concrete strategy,
// and the pointer indirection all exist to enable a swap that will never happen --
// speculative generality that adds a virtual call, an allocation, and a class for
// nothing.
struct TaxStrategy {
    virtual int taxOn(int amount) const = 0;
    virtual ~TaxStrategy() = default;
};

struct FlatTax : TaxStrategy {                         // the only strategy there will ever be
    int taxOn(int amount) const override { return amount / 5; }   // 20%
};

class InvoiceBad {
public:
    explicit InvoiceBad(std::unique_ptr<TaxStrategy> t) : tax_(std::move(t)) {}
    int tax(int amount) const { return tax_->taxOn(amount); }
private:
    std::unique_ptr<TaxStrategy> tax_;
};

// FIX: with one algorithm and no second in sight, it is just a method. Introduce
// the Strategy when a real second rule appears -- then the abstraction is shaped
// by two concrete cases, not guessed from one.
class InvoiceGood {
public:
    int tax(int amount) const { return amount / 5; }
};

int main() {
    InvoiceBad bad(std::unique_ptr<TaxStrategy>(new FlatTax));
    std::cout << "bad:  " << bad.tax(100) << "\n";    // 20

    InvoiceGood good;
    std::cout << "good: " << good.tax(100) << "\n";   // 20
    return 0;
}
