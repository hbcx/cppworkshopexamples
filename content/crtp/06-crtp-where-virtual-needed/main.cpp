// Anti-pattern: CRTP where runtime polymorphism is actually needed.
//
// CRTP freezes the type at compile time, so Widget<Button> and Widget<Slider>
// have no common base: you cannot put both in one container, and you cannot
// return "whichever widget the config names" from a function. Those are exactly
// what dynamic polymorphism is for.
//
// The fix is a virtual base: one Widget interface, uniform storage through
// unique_ptr<Widget>, and a factory that chooses the type at run time.

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ---------- CRTP: no common base, so no heterogeneous storage ----------
namespace bad {

template <class Derived>
struct Widget {
    void render() const { static_cast<const Derived&>(*this).render(); }
};

struct Button : Widget<Button> {
    void render() const { std::cout << "[button]\n"; }
};
struct Slider : Widget<Slider> {
    void render() const { std::cout << "[slider]\n"; }
};

// There is NO type that holds both Button and Slider:
//     std::vector<Widget<?>> ui;   // <-- cannot be written: which Derived?
// and no factory can return "a widget" chosen from a value, because the return
// type would have to be fixed at compile time.

} // namespace bad

// ---------- virtual: one base, uniform storage, runtime choice ----------
namespace good {

struct Widget {
    virtual ~Widget() {}
    virtual void render() const = 0;
};

struct Button : Widget {
    void render() const override { std::cout << "[button]\n"; }
};
struct Slider : Widget {
    void render() const override { std::cout << "[slider]\n"; }
};

// A factory can pick the concrete type from a runtime value.
std::unique_ptr<Widget> make(const std::string& kind) {
    if (kind == "button") return std::unique_ptr<Widget>(new Button);
    return std::unique_ptr<Widget>(new Slider);
}

} // namespace good

int main() {
    // Static CRTP widgets: each usable on its own, but not together.
    bad::Button b;
    bad::Slider s;
    std::cout << "CRTP widgets, one at a time:\n";
    b.render();
    s.render();

    // Virtual widgets: built from config, stored and rendered uniformly.
    std::cout << "virtual widgets from config:\n";
    const std::string config[] = {"button", "slider", "button"};
    std::vector<std::unique_ptr<good::Widget>> ui;
    for (const std::string& kind : config) ui.push_back(good::make(kind));
    for (const std::unique_ptr<good::Widget>& w : ui) w->render();
}
