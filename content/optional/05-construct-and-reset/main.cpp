#include <iostream>
#include <optional>
#include <string>
#include <utility>   // std::in_place

// A type that logs its constructor so we can see in-place construction happen
// without any temporary to move in.
struct Widget {
    Widget(int a, int b) { std::cout << "  Widget(" << a << ", " << b << ")\n"; }
};

int main() {
    std::cout << std::boolalpha;

    // make_optional deduces T and returns an engaged optional.
    auto n = std::make_optional(5);   // optional<int>, engaged with 5
    std::cout << "n = " << *n << '\n';

    // std::in_place builds the Widget directly inside the optional -- no
    // temporary Widget constructed and then moved in.
    std::cout << "in_place:\n";
    std::optional<Widget> w(std::in_place, 1, 2);

    // emplace destroys the current value and constructs a fresh one in place.
    std::cout << "emplace:\n";
    w.emplace(3, 4);

    // Assigning a value engages; nullopt / reset() disengages and destroys it.
    std::optional<std::string> s;
    s = "hello";
    std::cout << "s engaged? " << s.has_value() << ", *s = " << *s << '\n';
    s.reset();
    std::cout << "after reset, engaged? " << s.has_value() << '\n';
    s = std::nullopt;   // same effect as reset()
    std::cout << "after = nullopt, engaged? " << s.has_value() << '\n';
    return 0;
}
