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

// Anti-pattern: one free factory function per product, each keyed by the platform
// argument. The family choice is now repeated at every call and nothing forces
// the two arguments to agree.
std::unique_ptr<Button> makeButton(OS os) {
    if (os == OS::Windows) return std::make_unique<WindowsButton>();
    return std::make_unique<HtmlButton>();
}
std::unique_ptr<Checkbox> makeCheckbox(OS os) {
    if (os == OS::Windows) return std::make_unique<WindowsCheckbox>();
    return std::make_unique<HtmlCheckbox>();
}

// Fix: bind the family behind one factory object. Selecting the concrete factory
// once fixes every product, so a mismatched set cannot be expressed at all.
struct GUIFactory {
    virtual std::unique_ptr<Button> createButton() const = 0;
    virtual std::unique_ptr<Checkbox> createCheckbox() const = 0;
    virtual ~GUIFactory() = default;
};
struct WindowsFactory : GUIFactory {
    std::unique_ptr<Button> createButton() const override { return std::make_unique<WindowsButton>(); }
    std::unique_ptr<Checkbox> createCheckbox() const override { return std::make_unique<WindowsCheckbox>(); }
};

void renderPair(const std::string& label, const Button& b, const Checkbox& c) {
    std::cout << label << '\n';
    std::cout << "  " << b.render() << '\n';
    std::cout << "  " << c.render() << '\n';
}

int main() {
    // Trap: two independent calls, two different platforms -- it compiles and
    // runs, producing a Windows button above a web checkbox.
    std::unique_ptr<Button> b1 = makeButton(OS::Windows);
    std::unique_ptr<Checkbox> c1 = makeCheckbox(OS::Web);   // wrong family, nobody checked
    renderPair("[trap]  mismatched form:", *b1, *c1);

    // Fix: pick the factory once; the family is guaranteed to match.
    WindowsFactory factory;
    std::unique_ptr<Button> b2 = factory.createButton();
    std::unique_ptr<Checkbox> c2 = factory.createCheckbox();
    renderPair("[fix]   consistent form:", *b2, *c2);
    return 0;
}
