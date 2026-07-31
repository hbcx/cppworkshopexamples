#include <iostream>
#include <memory>
#include <string>

// Product: the interface the creator's algorithm depends on.
struct Button {
    virtual std::string render() const = 0;
    virtual ~Button() = default;   // deleted through the base -> must be virtual
};

// Concrete products.
struct WindowsButton : Button {
    std::string render() const override { return "[ OK ]"; }
};
struct HtmlButton : Button {
    std::string render() const override { return "<button>OK</button>"; }
};

// Creator: declares the factory method that subclasses override, and writes its
// real logic against the Button interface -- never naming a concrete product.
struct Dialog {
    virtual std::unique_ptr<Button> createButton() const = 0;   // the factory method
    virtual ~Dialog() = default;

    // Shared algorithm that USES the factory method: the varying construction is
    // deferred to the subclass, everything else lives here once.
    std::string renderWindow() const {
        std::unique_ptr<Button> button = createButton();
        return "Dialog with button: " + button->render();
    }
};

// Concrete creators decide which product to make.
struct WindowsDialog : Dialog {
    std::unique_ptr<Button> createButton() const override {
        return std::make_unique<WindowsButton>();
    }
};
struct WebDialog : Dialog {
    std::unique_ptr<Button> createButton() const override {
        return std::make_unique<HtmlButton>();
    }
};

int main() {
    std::unique_ptr<Dialog> dialog = std::make_unique<WindowsDialog>();
    std::cout << dialog->renderWindow() << '\n';

    dialog = std::make_unique<WebDialog>();
    std::cout << dialog->renderWindow() << '\n';
    return 0;
}
