#include <iostream>
#include <string>
#include <utility>

struct Widget {
    std::string name;
    int weight;

    Widget(std::string n, int w) : name(std::move(n)), weight(w) {
        std::cout << "Widget(" << name << ", " << weight << ")\n";
    }
};

// Forwards its arguments unchanged to T's constructor: an lvalue argument is
// passed on as an lvalue (copied), an rvalue as an rvalue (moved). This is
// exactly what emplace_back and std::make_unique do under the hood.
template <typename T, typename... Args>
T make(Args&&... args) {
    return T(std::forward<Args>(args)...);
}

int main() {
    std::string n = "gear";

    Widget a = make<Widget>(n, 5);                    // n is an lvalue -> name copied
    Widget b = make<Widget>(std::string("bolt"), 2);  // temporary  -> name moved

    std::cout << a.name << " " << b.name << '\n';
    return 0;
}
