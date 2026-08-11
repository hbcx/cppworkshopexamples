#include <iostream>
#include <type_traits>

struct Base {};
struct Derived : Base {};

int main() {
    std::cout << std::boolalpha;

    // is_convertible is DIRECTIONAL: A to B is not B to A.
    static_assert(std::is_convertible_v<Derived*, Base*>);    // up: yes
    static_assert(!std::is_convertible_v<Base*, Derived*>);   // down: no

    // And PERMISSIVE about numbers: BOTH directions convert, even though one loses
    // information.
    static_assert(std::is_convertible_v<int, double>);    // widening
    static_assert(std::is_convertible_v<double, int>);    // narrowing -- still convertible!

    // So is_convertible is NOT is_same and NOT interchangeable. For identity, use
    // is_same; for a direction, assert exactly the one you require.
    static_assert(!std::is_same_v<int, double>);

    std::cout << "int<->double both convertible: "
              << (std::is_convertible_v<int, double> && std::is_convertible_v<double, int>) << "\n";  // true
    std::cout << "but is_same<int, double>:       " << std::is_same_v<int, double> << "\n";            // false
    std::cout << "done\n";
    return 0;
}
