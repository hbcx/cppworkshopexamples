// Event bubbling: the chain is a parent hierarchy.
//
// A very common Chain of Responsibility hides in GUI toolkits: an event is
// delivered to the widget it happened on, and if that widget does not handle it,
// the event "bubbles up" to the parent, then the grandparent, until one consumes
// it or it reaches the top. The chain's "next" link is the parent pointer, and
// consuming an event (returning true) stops the propagation. Here a button sits
// in a panel inside a window, and different events are handled at different
// levels.

#include <iostream>
#include <string>
#include <utility>

class Widget {
    Widget* parent_;
    std::string name_;
public:
    Widget(std::string name, Widget* parent)
        : parent_(parent), name_(std::move(name)) {}
    virtual ~Widget() = default;

    void dispatch(const std::string& event) {
        if (onEvent(event)) {
            std::cout << name_ << " handled '" << event << "'\n";
            return; // consumed -> stop propagation
        }
        std::cout << name_ << " ignored '" << event << "', bubbling up\n";
        if (parent_)
            parent_->dispatch(event);
        else
            std::cout << "'" << event << "' reached the top unhandled\n";
    }

protected:
    virtual bool onEvent(const std::string&) { return false; } // default: not mine
};

class Window : public Widget {
public:
    explicit Window(std::string n) : Widget(std::move(n), nullptr) {}
    bool onEvent(const std::string& e) override { return e == "close"; }
};
class Panel : public Widget {
public:
    Panel(std::string n, Widget* p) : Widget(std::move(n), p) {}
    bool onEvent(const std::string& e) override { return e == "menu"; }
};
class Button : public Widget {
public:
    Button(std::string n, Widget* p) : Widget(std::move(n), p) {}
    bool onEvent(const std::string& e) override { return e == "click"; }
};

int main() {
    Window window("window");
    Panel panel("panel", &window);
    Button button("button", &panel); // button -> panel -> window (parent chain)

    const std::string events[] = { "click", "menu", "close", "resize" };
    for (const std::string& e : events) {
        std::cout << "-- dispatch '" << e << "' at button --\n";
        button.dispatch(e);
    }
    return 0;
}
