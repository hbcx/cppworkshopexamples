// Perfect forwarding generalises to any number of arguments with a parameter
// pack: Args&&... args deduces each argument's category independently, and
// std::forward<Args>(args)... passes the whole list on unchanged. This is how
// factory functions (std::make_unique, emplace_back) construct an object in place
// without adding a copy or move to any argument.
#include <iostream>
#include <utility>
#include <string>

struct Widget {
    Widget(const std::string& name, int id) {
        std::cout << "Widget(" << name << ", " << id << ") built in place\n";
    }
};

// Construct a T from forwarded arguments -- a tiny make_unique-style factory.
template <typename T, typename... Args>
T create(Args&&... args) {
    return T(std::forward<Args>(args)...);
}

int main() {
    std::string n = "gear";
    Widget w = create<Widget>(n, 42);     // n forwarded as an lvalue, 42 as an rvalue
    (void)w;
    return 0;
}
