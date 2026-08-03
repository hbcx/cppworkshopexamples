#include <iostream>
#include <memory>
#include <string>

struct Style {
    std::string font;
};

// ANTI-PATTERN: a widget holds a RAW pointer to a flyweight owned elsewhere. It
// is valid only while that owner keeps the flyweight alive. If the factory that
// owns the styles is destroyed, or evicts one, every widget still pointing at it
// dangles -- and reading through it is undefined behaviour.
struct WidgetBad {
    const Style* style;      // non-owning -- dangles if the owner goes away
};

// FIX: the widget SHARES ownership of the flyweight through shared_ptr. The style
// lives as long as any widget references it, so it cannot dangle even after the
// factory that created it is gone.
struct WidgetGood {
    std::shared_ptr<const Style> style;
};

int main() {
    // The raw-pointer form is safe ONLY while the owner outlives the widget.
    Style shared{"Sans"};
    WidgetBad wb{&shared};
    std::cout << "bad widget font: " << wb.style->font << "\n";   // fine: shared still alive
    // If 'shared' (or a factory owning it) were destroyed before wb, wb.style
    // would dangle and reading wb.style->font would be undefined behaviour -- so
    // that case is described, not run.

    // The owning form survives its creator.
    std::shared_ptr<const Style> kept;
    {
        auto factory = std::make_shared<Style>(Style{"Serif"});   // a local owner
        WidgetGood wg{factory};
        kept = wg.style;              // the widget shares ownership of the style
    }   // the factory's own handle goes away here...
    std::cout << "good widget font: " << kept->font << "\n";       // ...but the style lives on
    return 0;
}
