// The real inversion: the high-level module OWNS the port and states it in its
// own terms; the low-level detail conforms to it.
//
// OrderService (policy) declares PaymentPort with charge(cents) -- an amount and
// a yes/no, nothing about any provider. StripeAdapter (detail) implements it.
// The adapter depends on the port that lives with the policy, so the dependency
// arrow points from the detail toward the abstraction, not the other way round.

#include <iostream>
#include <string>

// --- high-level policy layer: it declares the port it needs, in domain terms ---
class PaymentPort {
public:
    virtual ~PaymentPort() {}
    virtual bool charge(long cents) = 0;
};

class OrderService {
public:
    explicit OrderService(PaymentPort& payment) : payment_(payment) {}
    std::string checkout(long cents) {
        return payment_.charge(cents) ? "order placed" : "payment declined";
    }
private:
    PaymentPort& payment_;
};

// --- low-level detail layer: the adapter conforms to the policy's port ---
class StripeAdapter : public PaymentPort {
public:
    bool charge(long cents) override {
        // A real adapter would build a Stripe request here; the vendor's
        // vocabulary stays on THIS side of the port.
        return cents <= 100000; // pretend anything up to $1000 succeeds
    }
};

int main() {
    StripeAdapter stripe;
    OrderService orders(stripe);

    std::cout << "charge $50.00  -> " << orders.checkout(5000) << "\n";
    std::cout << "charge $2000.00 -> " << orders.checkout(200000) << "\n";
    // Swapping to PayPal would be a new adapter to the same PaymentPort;
    // OrderService would not change and needs no payment library to compile.
}
