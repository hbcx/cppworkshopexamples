#include <iostream>

// Anti-pattern: calling a virtual from a constructor. The base constructor runs
// before the derived part exists, so the virtual resolves to the BASE version --
// the override is not reached.

class Widget {
public:
    Widget() {
        // Looks like it will call the derived name(); it does not.
        std::cout << "constructing a " << name() << "\n";
    }
    virtual const char* name() const { return "Widget"; }
    virtual ~Widget() = default;
};

class Button : public Widget {
public:
    const char* name() const override { return "Button"; }
};

class SafeButton : public Widget {
public:
    // FIX: do the name-dependent work AFTER construction, where dispatch is normal.
    void announce() const { std::cout << "ready: a " << name() << "\n"; }
    const char* name() const override { return "SafeButton"; }
};

int main() {
    // The base constructor runs during this line and prints "Widget", not "Button":
    Button b;                       // constructing a Widget
    (void)b;

    // After construction, virtual dispatch works as expected:
    SafeButton s;                   // constructing a Widget   (still, in the ctor)
    s.announce();                   // ready: a SafeButton     (correct, post-construction)
    return 0;
}
