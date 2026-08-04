// Hooks: optional steps with a default the subclass MAY override.
//
// Not every step must be supplied by the subclass. A pure virtual is a REQUIRED
// step (the subclass must define it); a non-pure virtual with a default body is
// a HOOK -- an optional step the subclass may override to customise, or ignore
// to accept the default. A hook that returns a value can even steer the skeleton
// itself: wantsExtras() below defaults to true, and a subclass overrides it to
// false to opt a whole step out, without the base knowing why.

#include <iostream>

class Beverage {
public:
    virtual ~Beverage() = default;

    void prepare() {
        boilWater();
        brew();                     // required
        pourInCup();
        if (wantsExtras())          // a hook decides whether this step runs
            addExtras();            // a hook provides its content (default: none)
    }

protected:
    virtual void brew() = 0;                 // required step: pure virtual

    // Hooks: non-pure virtuals with defaults. Override only if you want to.
    virtual bool wantsExtras() { return true; }
    virtual void addExtras()   { std::cout << "add nothing\n"; }

private:
    void boilWater() { std::cout << "boil water\n"; }
    void pourInCup() { std::cout << "pour into cup\n"; }
};

// Overrides the content hook, keeps the default "yes, add extras".
class Tea : public Beverage {
protected:
    void brew() override      { std::cout << "steep the tea\n"; }
    void addExtras() override { std::cout << "add lemon\n"; }
};

// Overrides the boolean hook to skip the extras step entirely; leaves addExtras
// at its default because it is never called.
class BlackCoffee : public Beverage {
protected:
    void brew() override       { std::cout << "brew the grounds\n"; }
    bool wantsExtras() override { return false; }
};

int main() {
    std::cout << "-- tea --\n";
    Tea{}.prepare();
    std::cout << "-- black coffee --\n";
    BlackCoffee{}.prepare();
    return 0;
}
