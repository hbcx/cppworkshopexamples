#include <iostream>
#include <memory>
#include <string>
#include <utility>

int copies = 0;
struct Tracked {
    Tracked() = default;
    Tracked(const Tracked&) { ++copies; }
    Tracked(Tracked&&) noexcept = default;
};

struct Widget {
    Tracked t;
    std::string name;
    Widget(Tracked tr, std::string n) : t(std::move(tr)), name(std::move(n)) {}
};

// A variadic factory that PERFECTLY FORWARDS its arguments to the constructor:
// forwarding references Args&&... capture value category, std::forward preserves
// it, and the pack expands the forward per element.
template <class T, class... Args>
std::unique_ptr<T> makeUnique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

int main() {
    copies = 0;
    auto w = makeUnique<Widget>(Tracked{}, std::string("gadget"));   // rvalue args -> moved
    std::cout << "copies = " << copies << " (perfect forwarding copied nothing)\n";  // 0
    std::cout << "name = " << w->name << "\n";
    return 0;
}
