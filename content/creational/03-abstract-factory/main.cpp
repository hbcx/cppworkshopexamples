#include <iostream>
#include <memory>
#include <string>

// Two product families of related widgets. The client works only through these
// interfaces, never the concrete classes.
struct Button {
    virtual std::string render() const = 0;
    virtual ~Button() = default;
};
struct Checkbox {
    virtual std::string render() const = 0;
    virtual ~Checkbox() = default;
};

// --- Windows family ---
struct WindowsButton : Button {
    std::string render() const override { return "[ OK ]"; }
};
struct WindowsCheckbox : Checkbox {
    std::string render() const override { return "[x] Enabled"; }
};

// --- Web family ---
struct HtmlButton : Button {
    std::string render() const override { return "<button>OK</button>"; }
};
struct HtmlCheckbox : Checkbox {
    std::string render() const override { return "<input type=checkbox checked>"; }
};

// The abstract factory: one creator per product KIND. A concrete factory must
// return members of one coherent family, so the products always match.
struct GUIFactory {
    virtual std::unique_ptr<Button> createButton() const = 0;
    virtual std::unique_ptr<Checkbox> createCheckbox() const = 0;
    virtual ~GUIFactory() = default;
};

struct WindowsFactory : GUIFactory {
    std::unique_ptr<Button> createButton() const override {
        return std::make_unique<WindowsButton>();
    }
    std::unique_ptr<Checkbox> createCheckbox() const override {
        return std::make_unique<WindowsCheckbox>();
    }
};

struct WebFactory : GUIFactory {
    std::unique_ptr<Button> createButton() const override {
        return std::make_unique<HtmlButton>();
    }
    std::unique_ptr<Checkbox> createCheckbox() const override {
        return std::make_unique<HtmlCheckbox>();
    }
};

// Client code: it never names a concrete widget or family. Whichever factory it
// is handed, the button and checkbox it builds are guaranteed to be the same
// look -- there is no way to mix a Windows button with a web checkbox here.
void renderForm(const std::string& label, const GUIFactory& factory) {
    std::unique_ptr<Button> button = factory.createButton();
    std::unique_ptr<Checkbox> checkbox = factory.createCheckbox();
    std::cout << "[" << label << "] Rendering form:\n";
    std::cout << "  " << button->render() << '\n';
    std::cout << "  " << checkbox->render() << '\n';
}

int main() {
    WindowsFactory windows;
    WebFactory web;

    // Choosing the factory once fixes the WHOLE product set to one family.
    renderForm("windows", windows);
    renderForm("web", web);
    return 0;
}
