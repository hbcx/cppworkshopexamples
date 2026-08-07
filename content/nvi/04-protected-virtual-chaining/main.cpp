#include <iostream>
#include <string>

class Widget {
public:
    virtual ~Widget() = default;

    // Public, non-virtual entry: wraps the whole thing in braces, then calls the
    // protected virtual chain.
    std::string serialize() const {
        return "{" + fields() + "}";
    }

protected:
    // PROTECTED (not private) virtual: derived overrides call the base version
    // to add their own fields on top of the common ones.
    virtual std::string fields() const { return "id=1"; }
};

class Button : public Widget {
protected:
    std::string fields() const override {
        return Widget::fields() + ",label=OK";     // chain to the base's fields
    }
};

class ToggleButton : public Button {
protected:
    std::string fields() const override {
        return Button::fields() + ",on=true";      // chain again
    }
};

int main() {
    std::cout << Widget{}.serialize() << "\n";
    std::cout << Button{}.serialize() << "\n";
    std::cout << ToggleButton{}.serialize() << "\n";
    return 0;
}
