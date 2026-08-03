#include <functional>
#include <iostream>
#include <utility>

// The strategy is just a callable: no interface, no class per algorithm. The
// context stores a std::function and delegates to it, so a lambda, a free
// function, or a functor all plug in the same way -- the modern C++ Strategy.
class Checkout {
public:
    explicit Checkout(std::function<int(int)> pricing) : pricing_(std::move(pricing)) {}
    void setPricing(std::function<int(int)> pricing) { pricing_ = std::move(pricing); }
    int total(int base) const { return pricing_(base); }
private:
    std::function<int(int)> pricing_;
};

int regularPrice(int base) { return base; }   // a plain free function works too

int main() {
    Checkout cart(regularPrice);
    std::cout << "regular:   " << cart.total(100) << "\n";     // 100

    cart.setPricing([](int b) { return b - b / 10; });         // 10% off, as a lambda
    std::cout << "member:    " << cart.total(100) << "\n";     // 90

    int coupon = 50;                                           // a mutable value, genuinely captured
    cart.setPricing([coupon](int b) { return b - coupon; });   // a lambda that captures state
    std::cout << "coupon 50: " << cart.total(100) << "\n";     // 50
    return 0;
}
