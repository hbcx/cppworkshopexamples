#include <iostream>
#include <stdexcept>
#include <variant>

// A type whose MOVE constructor throws (and so is not noexcept). Because it is
// not nothrow-movable, changing a variant's alternative to it cannot use the
// safe "build a temporary and move it back" path -- so a throw mid-assignment
// leaves the variant valueless. (A controlled throw, not undefined behaviour.)
struct Fragile {
    Fragile() = default;
    Fragile(const Fragile&) = default;
    Fragile(Fragile&&) { throw std::runtime_error("move failed"); }
    Fragile& operator=(Fragile&&) = default;
};

int main() {
    std::cout << std::boolalpha;
    std::variant<int, Fragile> v = 1;
    std::cout << "before: valueless? " << v.valueless_by_exception() << '\n';   // false

    // The hazard: assigning an alternative whose construction (here a move)
    // throws leaves the variant with NO active alternative -- the old int is
    // gone and the Fragile never completed. index() becomes variant_npos.
    try {
        v = Fragile{};              // move-constructing the Fragile throws
    } catch (const std::runtime_error&) {
        std::cout << "caught move failure\n";
    }
    std::cout << "after:  valueless? " << v.valueless_by_exception() << '\n';    // true

    // Assuming it still holds a value is the bug: std::visit throws on a
    // valueless variant.
    try {
        std::visit([](auto&&) {}, v);
    } catch (const std::bad_variant_access&) {
        std::cout << "visit on valueless threw bad_variant_access\n";
    }

    // Avoidance/recovery: prefer nothrow-movable alternatives (so assignment can
    // roll back to the old value), and re-establish a known state after catching.
    v = 0;
    std::cout << "restored: valueless? " << v.valueless_by_exception() << '\n';  // false
    return 0;
}
