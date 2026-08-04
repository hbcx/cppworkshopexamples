// Template Method basics: a fixed skeleton with overridable steps.
//
// The base class owns the ALGORITHM -- the order of steps -- in a single method
// (prepare()), and defers only the steps that vary to virtual functions the
// subclasses fill in. boilWater() and pourInCup() are the same for every drink
// and live in the base; brew() and addExtras() differ and are overridden. The
// subclass never sees or controls the order: the base calls down into it. That
// inversion -- "don't call us, we'll call you" -- is the Hollywood principle,
// and it is what lets the base guarantee the recipe is always followed.

#include <iostream>

class Beverage {
public:
    virtual ~Beverage() = default;

    // The template method: the fixed skeleton. Note it is NOT virtual -- the
    // algorithm's shape is the base's invariant, not something a subclass edits.
    void prepare() {
        boilWater();
        brew();       // varies
        pourInCup();
        addExtras();  // varies
    }

protected:
    // The varying steps subclasses must supply.
    virtual void brew() = 0;
    virtual void addExtras() = 0;

private:
    // The common steps, fixed for every beverage.
    void boilWater()  { std::cout << "boil water\n"; }
    void pourInCup()  { std::cout << "pour into cup\n"; }
};

class Tea : public Beverage {
protected:
    void brew() override      { std::cout << "steep the tea\n"; }
    void addExtras() override { std::cout << "add lemon\n"; }
};

class Coffee : public Beverage {
protected:
    void brew() override      { std::cout << "brew the grounds\n"; }
    void addExtras() override { std::cout << "add sugar and milk\n"; }
};

int main() {
    std::cout << "-- tea --\n";
    Tea{}.prepare();
    std::cout << "-- coffee --\n";
    Coffee{}.prepare();
    return 0;
}
