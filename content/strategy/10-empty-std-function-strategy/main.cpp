#include <functional>
#include <iostream>
#include <utility>

// ANTI-PATTERN: the context default-constructs its std::function strategy, so it
// is EMPTY until someone sets it. Invoking an empty std::function throws
// std::bad_function_call -- a runtime failure hiding behind a valid-looking object
// that compiled cleanly and constructed without complaint.
class CheckoutBad {
public:
    void setPricing(std::function<int(int)> p) { pricing_ = std::move(p); }
    int total(int base) const { return pricing_(base); }   // throws if never set
private:
    std::function<int(int)> pricing_;   // empty by default -- a bug waiting to fire
};

// FIX: never let the strategy be empty. Require it in the constructor (best), so
// an unset strategy cannot exist; defaulting it to a sensible strategy, or
// guarding the call with if (pricing_), are the other options.
class CheckoutGood {
public:
    explicit CheckoutGood(std::function<int(int)> p) : pricing_(std::move(p)) {}
    int total(int base) const { return pricing_(base); }
private:
    std::function<int(int)> pricing_;
};

int main() {
    std::cout << "== empty strategy invoked ==\n";
    CheckoutBad bad;                       // pricing never set
    try {
        bad.total(100);                    // throws std::bad_function_call
    } catch (const std::bad_function_call&) {
        std::cout << "caught: std::bad_function_call\n";
    }

    std::cout << "== strategy required at construction ==\n";
    CheckoutGood good([](int b) { return b - b / 10; });
    std::cout << "member: " << good.total(100) << "\n";   // 90
    return 0;
}
