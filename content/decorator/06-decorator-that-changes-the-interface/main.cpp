#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Component interface: what every client and every decorator agrees on.
struct Coffee {
    virtual int cost() const = 0;            // cents
    virtual std::string desc() const = 0;
    virtual ~Coffee() = default;
};

class Espresso : public Coffee {
public:
    int cost() const override { return 200; }
    std::string desc() const override { return "espresso"; }
};

// ANTI-PATTERN: this decorator adds a PUBLIC method, sizeOz(), that is not on
// the Coffee interface. It is reachable only while you hold this object by its
// concrete type. The moment it is wrapped by another decorator -- or passed
// around as a Coffee& -- the extra method is gone, because the interface is the
// only thing that survives stacking.
class SizedBad : public Coffee {
public:
    SizedBad(std::unique_ptr<Coffee> c, int oz) : c_(std::move(c)), oz_(oz) {}
    int cost() const override { return c_->cost() + 30; }
    std::string desc() const override { return c_->desc() + " (large)"; }
    int sizeOz() const { return oz_; }        // NOT part of Coffee
private:
    std::unique_ptr<Coffee> c_;
    int oz_;
};

// FIX: if clients need the size, it belongs on the Component interface, so every
// layer forwards it and it survives any amount of stacking. A decorator adds
// behaviour to existing operations; it must not smuggle in new ones.
struct Drink {
    virtual int cost() const = 0;
    virtual std::string desc() const = 0;
    virtual int sizeOz() const = 0;          // now part of the contract
    virtual ~Drink() = default;
};

class Tea : public Drink {
public:
    int cost() const override { return 150; }
    std::string desc() const override { return "tea"; }
    int sizeOz() const override { return 8; }
};

class MilkGood : public Drink {
public:
    explicit MilkGood(std::unique_ptr<Drink> d) : d_(std::move(d)) {}
    int cost() const override { return d_->cost() + 50; }
    std::string desc() const override { return d_->desc() + " + milk"; }
    int sizeOz() const override { return d_->sizeOz(); }   // forward it
private:
    std::unique_ptr<Drink> d_;
};

int main() {
    // Broken: sizeOz() is reachable only on the bare concrete type...
    SizedBad big(std::make_unique<Espresso>(), 16);
    std::cout << big.desc() << " sizeOz=" << big.sizeOz() << '\n';

    // ...but wrap it and the size is gone: a Coffee* has no sizeOz() to call.
    std::unique_ptr<Coffee> wrapped =
        std::make_unique<SizedBad>(std::make_unique<Espresso>(), 16);
    // wrapped->sizeOz();   // COMPILE ERROR: Coffee has no member sizeOz
    std::cout << wrapped->desc() << " sizeOz=?? (hidden behind the interface)\n";

    // Fixed: sizeOz() is on the interface, so it survives stacking.
    std::unique_ptr<Drink> latte = std::make_unique<MilkGood>(std::make_unique<Tea>());
    std::cout << latte->desc() << " sizeOz=" << latte->sizeOz() << '\n';
    return 0;
}
