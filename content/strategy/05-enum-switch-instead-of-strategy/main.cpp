#include <iostream>

// ANTI-PATTERN: the context branches on an enum "mode" with a switch inside its
// own method. Every new pricing rule edits this switch (and every other method
// that switches on the mode), so the class that should be stable changes on every
// addition -- the open/closed violation Strategy exists to remove.
class CheckoutBad {
public:
    enum class Mode { Regular, Member, Clearance };
    explicit CheckoutBad(Mode m) : mode_(m) {}
    int total(int base) const {
        switch (mode_) {
            case Mode::Regular:   return base;
            case Mode::Member:    return base - base / 10;
            case Mode::Clearance: return base / 2;
        }
        return base;   // a newly added mode silently falls through here if forgotten
    }
private:
    Mode mode_;
};

// FIX: each algorithm is a strategy; the context delegates and never switches. A
// new rule is a new class the context never sees or edits.
struct Pricing {
    virtual int priceFor(int base) const = 0;
    virtual ~Pricing() = default;
};
struct Regular : Pricing { int priceFor(int b) const override { return b; } };
struct Member : Pricing { int priceFor(int b) const override { return b - b / 10; } };
struct Clearance : Pricing { int priceFor(int b) const override { return b / 2; } };

class CheckoutGood {
public:
    explicit CheckoutGood(const Pricing& p) : pricing_(&p) {}
    int total(int base) const { return pricing_->priceFor(base); }
private:
    const Pricing* pricing_;
};

int main() {
    std::cout << "== enum + switch ==\n";
    std::cout << "member: " << CheckoutBad(CheckoutBad::Mode::Member).total(100) << "\n";   // 90

    std::cout << "== strategy ==\n";
    Member member;
    std::cout << "member: " << CheckoutGood(member).total(100) << "\n";   // 90
    return 0;
}
