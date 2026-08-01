#include <iostream>
#include <string>

struct Gateway {
    virtual std::string charge(int cents) = 0;
    virtual ~Gateway() = default;
};

struct RealGateway : Gateway {
    std::string charge(int cents) override {
        return "charged " + std::to_string(cents) + " via bank";
    }
};

struct FakeGateway : Gateway {
    std::string charge(int cents) override {
        return "fake-ok " + std::to_string(cents);
    }
};

// A global service locator: a hidden well any code can reach into.
struct Locator {
    static Gateway* gateway;
};
Gateway* Locator::gateway = nullptr;

// Anti-pattern: the dependency is pulled from the global locator inside the
// method. checkout() takes no gateway -- its signature lies about what it needs,
// and a test must mutate global state (which then leaks to the next test) to
// substitute a fake.
struct CheckoutLocated {
    std::string checkout(int cents) {
        return Locator::gateway->charge(cents);   // hidden dependency
    }
};

// Fix: inject the gateway. The dependency is declared in the type, supplied
// locally, and substitutable with no global state at all.
class CheckoutInjected {
public:
    explicit CheckoutInjected(Gateway& gateway) : gateway_(gateway) {}
    std::string checkout(int cents) { return gateway_.charge(cents); }
private:
    Gateway& gateway_;
};

int main() {
    // Located: global state must be set before use, and it stays set afterward.
    RealGateway real;
    Locator::gateway = &real;
    CheckoutLocated located;
    std::cout << "located  -> " << located.checkout(500) << '\n';
    std::cout << "(global Locator::gateway still set: "
              << (Locator::gateway ? "yes" : "no") << ")\n";

    // Injected: the dependency is passed in, visible in the type, and a fake is
    // supplied locally with nothing global touched.
    FakeGateway fake;
    CheckoutInjected injected(fake);
    std::cout << "injected -> " << injected.checkout(500) << '\n';
    return 0;
}
