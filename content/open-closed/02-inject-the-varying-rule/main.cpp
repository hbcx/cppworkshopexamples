// Open-Closed at runtime -- inject the behaviour that varies (this is Strategy).
//
// A checkout must support new discount rules over time. Instead of editing the
// checkout for each one, the varying rule is injected as a callable. A new
// discount is a new function; Checkout -- the closed part -- never changes.

#include <functional>
#include <iostream>
#include <string>
#include <utility>

using Discount = std::function<long(long)>; // cents off, given the subtotal

class Checkout {
public:
    explicit Checkout(Discount d) : discount_(std::move(d)) {}

    long totalCents(long subtotalCents) const {
        return subtotalCents - discount_(subtotalCents);
    }

private:
    Discount discount_;
};

static std::string money(long cents) {
    std::string f = std::to_string(cents % 100);
    if (f.size() < 2) f = "0" + f;
    return "$" + std::to_string(cents / 100) + "." + f;
}

int main() {
    const long subtotal = 10000; // $100.00

    Discount none    = [](long) { return 0L; };
    Discount tenPct  = [](long s) { return s / 10; };
    Discount loyalty = [](long s) { return s >= 5000 ? 500L : 0L; };

    std::cout << "no discount: " << money(Checkout(none).totalCents(subtotal))    << "\n";
    std::cout << "ten percent: " << money(Checkout(tenPct).totalCents(subtotal))  << "\n";
    std::cout << "loyalty:     " << money(Checkout(loyalty).totalCents(subtotal)) << "\n";

    // EXTENSION: a Black Friday rule added later -- Checkout is not touched.
    Discount blackFriday = [](long s) { return s / 2; };
    std::cout << "black friday: " << money(Checkout(blackFriday).totalCents(subtotal)) << "\n";
}
