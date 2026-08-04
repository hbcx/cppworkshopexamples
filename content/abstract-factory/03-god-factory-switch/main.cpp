#include <iostream>
#include <memory>
#include <string>

struct Button {
    virtual std::string render() const = 0;
    virtual ~Button() = default;
};
struct Checkbox {
    virtual std::string render() const = 0;
    virtual ~Checkbox() = default;
};

struct WindowsButton : Button {
    std::string render() const override { return "[ OK ]"; }
};
struct WindowsCheckbox : Checkbox {
    std::string render() const override { return "[x] Enabled"; }
};
struct HtmlButton : Button {
    std::string render() const override { return "<button>OK</button>"; }
};
struct HtmlCheckbox : Checkbox {
    std::string render() const override { return "<input type=checkbox checked>"; }
};

enum class OS { Windows, Web };

// Anti-pattern: one factory holding a platform enum and switching on it inside
// EVERY create method. Adding a family means editing every method to add a case;
// adding a product means a fresh method that again switches on all families. The
// class depends on every widget of every family at once.
struct GodFactory {
    OS os;
    explicit GodFactory(OS o) : os(o) {}
    std::unique_ptr<Button> createButton() const {
        switch (os) {
            case OS::Windows: return std::make_unique<WindowsButton>();
            case OS::Web:     return std::make_unique<HtmlButton>();
        }
        return nullptr;
    }
    std::unique_ptr<Checkbox> createCheckbox() const {
        switch (os) {
            case OS::Windows: return std::make_unique<WindowsCheckbox>();
            case OS::Web:     return std::make_unique<HtmlCheckbox>();
        }
        return nullptr;
    }
};

// Fix: one concrete factory per family, switching on nothing. A new family is a
// new subclass the client never names; the compiler forces each factory to
// supply the whole product set.
struct GUIFactory {
    virtual std::unique_ptr<Button> createButton() const = 0;
    virtual std::unique_ptr<Checkbox> createCheckbox() const = 0;
    virtual ~GUIFactory() = default;
};
struct WindowsFactory : GUIFactory {
    std::unique_ptr<Button> createButton() const override { return std::make_unique<WindowsButton>(); }
    std::unique_ptr<Checkbox> createCheckbox() const override { return std::make_unique<WindowsCheckbox>(); }
};
struct WebFactory : GUIFactory {
    std::unique_ptr<Button> createButton() const override { return std::make_unique<HtmlButton>(); }
    std::unique_ptr<Checkbox> createCheckbox() const override { return std::make_unique<HtmlCheckbox>(); }
};

// The switch belongs at ONE point: where the family is first chosen.
std::unique_ptr<GUIFactory> makeFactory(OS os) {
    switch (os) {
        case OS::Windows: return std::make_unique<WindowsFactory>();
        case OS::Web:     return std::make_unique<WebFactory>();
    }
    return nullptr;
}

void showForm(const std::string& label, const GUIFactory& f) {
    std::cout << label << '\n';
    std::cout << "  " << f.createButton()->render() << '\n';
    std::cout << "  " << f.createCheckbox()->render() << '\n';
}

int main() {
    // God factory: works, but the switch is spread across every method.
    GodFactory god(OS::Windows);
    std::cout << "[god]   form:\n";
    std::cout << "  " << god.createButton()->render() << '\n';
    std::cout << "  " << god.createCheckbox()->render() << '\n';

    // Polymorphic factories: one subclass per family, switch only at selection.
    std::unique_ptr<GUIFactory> windows = makeFactory(OS::Windows);
    std::unique_ptr<GUIFactory> web = makeFactory(OS::Web);
    showForm("[poly]  windows form:", *windows);
    showForm("[poly]  web form:", *web);
    return 0;
}
