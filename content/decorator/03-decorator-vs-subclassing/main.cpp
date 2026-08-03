#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Component: a beverage priced in cents, with a description.
struct Beverage {
    virtual int cost() const = 0;
    virtual std::string desc() const = 0;
    virtual ~Beverage() = default;
};

// Concrete component.
class Espresso : public Beverage {
public:
    int cost() const override { return 200; }
    std::string desc() const override { return "espresso"; }
};

// Base decorator: owns the wrapped beverage.
class Condiment : public Beverage {
public:
    explicit Condiment(std::unique_ptr<Beverage> b) : b_(std::move(b)) {}
protected:
    const Beverage& base() const { return *b_; }
private:
    std::unique_ptr<Beverage> b_;
};

// Each condiment adds ITS price and name to whatever it wraps. The subclassing
// alternative would need one class per COMBINATION -- EspressoWithMilk,
// EspressoWithMilkAndSugar, EspressoWithDoubleMilk, ... -- a set that multiplies
// with every new drink and every new condiment. Here each condiment is written
// once and combinations are built at runtime.
class Milk : public Condiment {
public:
    using Condiment::Condiment;
    int cost() const override { return base().cost() + 50; }
    std::string desc() const override { return base().desc() + " + milk"; }
};

class Sugar : public Condiment {
public:
    using Condiment::Condiment;
    int cost() const override { return base().cost() + 10; }
    std::string desc() const override { return base().desc() + " + sugar"; }
};

class WhippedCream : public Condiment {
public:
    using Condiment::Condiment;
    int cost() const override { return base().cost() + 70; }
    std::string desc() const override { return base().desc() + " + cream"; }
};

void printOrder(const Beverage& b) {
    std::cout << b.desc() << " = " << b.cost() << " cents\n";
}

int main() {
    auto plain = std::make_unique<Espresso>();
    printOrder(*plain);                                    // espresso = 200 cents

    // Compose condiments at runtime -- no EspressoWithMilkAndSugar class needed.
    auto sweet = std::make_unique<Sugar>(
                     std::make_unique<Milk>(
                         std::make_unique<Espresso>()));
    printOrder(*sweet);                                    // espresso + milk + sugar = 260 cents

    // Decorators can even apply the SAME condiment twice -- double milk -- which
    // a fixed subclass hierarchy cannot express at all.
    auto rich = std::make_unique<Milk>(
                    std::make_unique<Milk>(
                        std::make_unique<WhippedCream>(
                            std::make_unique<Espresso>())));
    printOrder(*rich);                                     // espresso + cream + milk + milk = 370 cents
    return 0;
}
