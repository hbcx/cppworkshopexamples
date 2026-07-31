#include <functional>
#include <iostream>
#include <memory>
#include <string>

// Product interface (same as the Factory Method example).
struct Button {
    virtual std::string render() const = 0;
    virtual ~Button() = default;
};
struct WindowsButton : Button {
    std::string render() const override { return "[ OK ]"; }
};
struct HtmlButton : Button {
    std::string render() const override { return "<button>OK</button>"; }
};

// Anti-pattern: a whole Creator SUBCLASS per product, when the only thing that
// varies is the one line that builds the button. Each new product needs a new
// Dialog subclass overriding createButton and nothing else.
struct Dialog {
    virtual std::unique_ptr<Button> createButton() const = 0;
    virtual ~Dialog() = default;
    std::string renderWindow() const { return "Dialog: " + createButton()->render(); }
};
struct WindowsDialog : Dialog {
    std::unique_ptr<Button> createButton() const override { return std::make_unique<WindowsButton>(); }
};
struct WebDialog : Dialog {
    std::unique_ptr<Button> createButton() const override { return std::make_unique<HtmlButton>(); }
};

// Fix: parameterize the dialog with a factory CALLABLE. No subclassing -- a new
// product is just a different lambda, and the varying construction is data, not
// a type. Reach for a subclass only when it changes more than the created type.
struct ConfigurableDialog {
    std::function<std::unique_ptr<Button>()> makeButton;
    std::string renderWindow() const { return "Dialog: " + makeButton()->render(); }
};

int main() {
    // Heavyweight: one subclass per product.
    std::unique_ptr<Dialog> a = std::make_unique<WindowsDialog>();
    std::unique_ptr<Dialog> b = std::make_unique<WebDialog>();
    std::cout << "[hierarchy] " << a->renderWindow() << '\n';
    std::cout << "[hierarchy] " << b->renderWindow() << '\n';

    // Lightweight: same behaviour, no subclasses -- the factory is a value.
    ConfigurableDialog win{ [] { return std::make_unique<WindowsButton>(); } };
    ConfigurableDialog web{ [] { return std::make_unique<HtmlButton>(); } };
    std::cout << "[function]  " << win.renderWindow() << '\n';
    std::cout << "[function]  " << web.renderWindow() << '\n';
    return 0;
}
