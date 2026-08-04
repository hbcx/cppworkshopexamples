// The canonical Mediator: a dialog coordinating its widgets.
//
// This is the example from the Gang of Four. Widgets in a form depend on each
// other -- the submit button should be enabled only when both fields are filled
// -- and without a mediator that rule would have to live inside the widgets, each
// referencing the others. Instead every widget knows only an abstract Mediator
// and reports "I changed"; the concrete dialog holds the coordination rule in one
// place. Depending on the Mediator INTERFACE (not the concrete dialog) keeps the
// widgets reusable with any mediator.

#include <iostream>
#include <string>
#include <utility>

class Widget;

class Mediator { // widgets depend on this, not on the concrete dialog
public:
    virtual ~Mediator() = default;
    virtual void widgetChanged(Widget* w) = 0;
};

class Widget {
protected:
    Mediator* mediator_;
    std::string name_;
public:
    Widget(std::string name, Mediator* m) : mediator_(m), name_(std::move(name)) {}
    virtual ~Widget() = default;
    const std::string& name() const { return name_; }
    void changed() { if (mediator_) mediator_->widgetChanged(this); }
};

class TextField : public Widget {
    std::string text_;
public:
    TextField(std::string name, Mediator* m) : Widget(std::move(name), m) {}
    void setText(const std::string& t) { text_ = t; changed(); }
    bool empty() const { return text_.empty(); }
};

class Button : public Widget {
    bool enabled_ = false;
public:
    Button(std::string name, Mediator* m) : Widget(std::move(name), m) {}
    void setEnabled(bool e) {
        if (e != enabled_) {
            enabled_ = e;
            std::cout << "  " << name() << (e ? " enabled" : " disabled") << "\n";
        }
    }
};

class LoginDialog : public Mediator { // the concrete mediator: holds the rule
    TextField user_;
    TextField pass_;
    Button submit_;
public:
    LoginDialog()
        : user_("username", this), pass_("password", this), submit_("submit", this) {}

    void typeUser(const std::string& s) { user_.setText(s); }
    void typePass(const std::string& s) { pass_.setText(s); }

    void widgetChanged(Widget*) override {
        // The interaction rule lives HERE, not scattered across the widgets.
        submit_.setEnabled(!user_.empty() && !pass_.empty());
    }
};

int main() {
    LoginDialog dlg;
    std::cout << "type username 'alice'\n";
    dlg.typeUser("alice"); // password still empty -> submit stays disabled
    std::cout << "type password 'secret'\n";
    dlg.typePass("secret"); // both filled -> submit enabled
    std::cout << "clear password\n";
    dlg.typePass(""); // -> submit disabled again
    return 0;
}
