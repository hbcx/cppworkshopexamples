#include <iostream>
#include <optional>
#include <string>

struct Widget {
    std::string name;
};

int main() {
    Widget w{"gadget"};
    std::cout << std::boolalpha;

    // Anti-pattern: optional<T*>. A pointer is ALREADY nullable -- nullptr means
    // "none" -- so wrapping it in optional creates TWO ways to say "no widget":
    // a disengaged optional, and an engaged optional holding nullptr.
    std::optional<Widget*> a;              // disengaged: "none" (version 1)
    std::optional<Widget*> b = nullptr;    // engaged, but holds nullptr (version 2)
    std::cout << "a engaged? " << a.has_value()
              << "   b engaged? " << b.has_value()
              << "   -- yet both mean \"no widget\"\n";

    // Fix: a plain pointer. nullptr is the single, conventional "none".
    Widget* p = &w;
    std::cout << "p -> " << (p ? p->name : std::string("none")) << '\n';
    p = nullptr;
    std::cout << "p -> " << (p ? p->name : std::string("none")) << '\n';

    // If you want value semantics (own the object, or "maybe a value" without a
    // raw pointer), use optional<Widget> -- exactly one empty state.
    std::optional<Widget> owned = Widget{"owned"};
    std::cout << "owned -> " << (owned ? owned->name : std::string("none")) << '\n';
    return 0;
}
