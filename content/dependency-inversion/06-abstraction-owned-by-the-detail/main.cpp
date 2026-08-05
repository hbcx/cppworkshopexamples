// Anti-pattern: the abstraction is owned (shaped) by the detail.
//
// PaymentGateway IS an interface, and OrderService depends on it -- yet its
// method takes a StripeToken, a vendor type. So the policy must build a
// StripeToken, names a vendor concept, and breaks if Stripe changes it. Routing
// the dependency through an interface did not remove it.
//
// The fix: the policy owns the port and states it in domain terms (charge cents);
// the adapter builds the vendor token internally, on its own side.

#include <iostream>
#include <string>

// ---------- the interface speaks the vendor's language ----------
namespace bad {

struct StripeToken { std::string value; }; // a detail type

class PaymentGateway {
public:
    virtual ~PaymentGateway() {}
    virtual bool stripeCharge(const StripeToken& token, long cents) = 0;
};

class OrderService {
public:
    explicit OrderService(PaymentGateway& gateway) : gateway_(gateway) {}
    std::string checkout(long cents) {
        StripeToken token{"tok_visa"}; // policy is forced to know StripeToken
        return gateway_.stripeCharge(token, cents) ? "order placed" : "declined";
    }
private:
    PaymentGateway& gateway_;
};

class StripeGateway : public PaymentGateway {
public:
    bool stripeCharge(const StripeToken&, long cents) override { return cents <= 100000; }
};

} // namespace bad

// ---------- the port speaks the policy's language ----------
namespace good {

class PaymentPort {
public:
    virtual ~PaymentPort() {}
    virtual bool charge(long cents) = 0; // domain terms only
};

class OrderService {
public:
    explicit OrderService(PaymentPort& payment) : payment_(payment) {}
    std::string checkout(long cents) {
        return payment_.charge(cents) ? "order placed" : "declined";
    }
private:
    PaymentPort& payment_;
};

class StripeAdapter : public PaymentPort {
public:
    bool charge(long cents) override {
        // The vendor token is built HERE, never seen by the policy.
        std::string token = "tok_visa";
        (void) token;
        return cents <= 100000;
    }
};

} // namespace good

int main() {
    bad::StripeGateway badGateway;
    bad::OrderService badOrders(badGateway);
    std::cout << "[bad]  " << badOrders.checkout(5000)
              << " (policy had to build a StripeToken)\n";

    good::StripeAdapter adapter;
    good::OrderService orders(adapter);
    std::cout << "[good] " << orders.checkout(5000)
              << " (policy knows only charge(cents))\n";
}
