// Anti-pattern: change amplification from low cohesion (shotgun surgery).
//
// The VAT rate 0.23 is hardcoded in four unrelated classes. Raising it is a
// four-file edit, and missing one leaves the price view, cart, invoice and
// receipt quietly disagreeing. "Knowing the tax rate" is one responsibility
// smeared across classes.
//
// The fix gives the rate a single owner -- TaxPolicy -- that every class asks,
// so the change is one edit and the numbers cannot drift apart.

#include <iostream>
#include <string>

static std::string dollars(long cents) {
    std::string frac = std::to_string(cents % 100);
    if (frac.size() < 2) frac = "0" + frac;
    return "$" + std::to_string(cents / 100) + "." + frac;
}

// ---------- the rate is copied into every class ----------
namespace bad {

class PriceView {
public:
    long withTax(long net) const { return net + static_cast<long>(net * 0.23); }
};
class CartSummary {
public:
    long withTax(long net) const { return net + static_cast<long>(net * 0.23); }
};
class Invoice {
public:
    long withTax(long net) const { return net + static_cast<long>(net * 0.23); }
};
class Receipt {
public:
    long withTax(long net) const { return net + static_cast<long>(net * 0.23); }
};

} // namespace bad

// ---------- one owner for the concept ----------
namespace good {

// The single source of truth for the tax rule. Change it here, once.
class TaxPolicy {
public:
    explicit TaxPolicy(int percent) : percent_(percent) {}
    long applyTo(long net) const { return net + net * percent_ / 100; }
private:
    int percent_;
};

class PriceView {
public:
    explicit PriceView(const TaxPolicy& t) : tax_(t) {}
    long withTax(long net) const { return tax_.applyTo(net); }
private:
    const TaxPolicy& tax_;
};

class Invoice {
public:
    explicit Invoice(const TaxPolicy& t) : tax_(t) {}
    long withTax(long net) const { return tax_.applyTo(net); }
private:
    const TaxPolicy& tax_;
};

} // namespace good

int main() {
    const long net = 10000; // $100.00

    // Four classes, four copies of the rate -- all agree only by luck.
    bad::PriceView pv; bad::CartSummary cs; bad::Invoice inv; bad::Receipt rc;
    std::cout << "[bad] view=" << dollars(pv.withTax(net))
              << " cart=" << dollars(cs.withTax(net))
              << " invoice=" << dollars(inv.withTax(net))
              << " receipt=" << dollars(rc.withTax(net))
              << "  (raising VAT = 4 edits)\n";

    // One policy, asked by everyone -- one edit to change, no drift.
    good::TaxPolicy tax(23);
    good::PriceView gpv(tax);
    good::Invoice ginv(tax);
    std::cout << "[good] view=" << dollars(gpv.withTax(net))
              << " invoice=" << dollars(ginv.withTax(net))
              << "  (raising VAT = 1 edit)\n";
}
