// Anti-pattern: a mediator for two objects that could just talk.
//
// Mediator pays off when many colleagues interact in tangled ways. For two
// objects with a single, simple interaction, the full apparatus -- a Mediator
// interface, colleague registration, a notify round-trip -- is pure ceremony
// around what a direct call (or one callback) does in a line. Below, a button
// updating a label is wrapped in a mediator for no benefit; the direct version
// is shorter, clearer, and just as decoupled for this scale.

#include <iostream>
#include <string>
#include <functional>

// ---- BAD: a mediator, a colleague base, registration -- to update one label ----
class Mediator {
public:
    virtual ~Mediator() = default;
    virtual void notify(const std::string& event) = 0;
};
class ButtonBad {
    Mediator* mediator_;
public:
    explicit ButtonBad(Mediator* m) : mediator_(m) {}
    void click() { mediator_->notify("clicked"); }
};
class LabelBad {
public:
    void setText(const std::string& t) { std::cout << "  label: " << t << "\n"; }
};
class DialogBad : public Mediator {
    LabelBad label_;
public:
    ButtonBad button{ this };
    void notify(const std::string& event) override {
        if (event == "clicked") label_.setText("clicked!");
    }
};

// ---- GOOD: the button just does the one thing, via a plain callback ----
class Label {
public:
    void setText(const std::string& t) { std::cout << "  label: " << t << "\n"; }
};
class Button {
    std::function<void()> onClick_;
public:
    explicit Button(std::function<void()> onClick) : onClick_(std::move(onClick)) {}
    void click() { onClick_(); }
};

int main() {
    std::cout << "-- bad (mediator for two) --\n";
    DialogBad dlg;
    dlg.button.click();

    std::cout << "-- good (direct) --\n";
    Label label;
    Button button([&label] { label.setText("clicked!"); });
    button.click();
    return 0;
}
