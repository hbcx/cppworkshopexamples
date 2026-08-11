#include <iostream>
#include <type_traits>

struct Base {};
struct Derived : Base {};
struct Other {};

int main() {
    std::cout << std::boolalpha;

    // is_same: are two types EXACTLY the same?
    static_assert(std::is_same_v<int, int>);
    static_assert(!std::is_same_v<int, const int>);   // const int is a different type
    static_assert(!std::is_same_v<int, long>);

    // is_base_of: is the first a base class of the second?
    static_assert(std::is_base_of_v<Base, Derived>);
    static_assert(!std::is_base_of_v<Derived, Base>);
    static_assert(!std::is_base_of_v<Other, Derived>);

    // is_convertible: can the first convert to the second? (directional)
    static_assert(std::is_convertible_v<Derived*, Base*>);    // up the hierarchy
    static_assert(!std::is_convertible_v<Base*, Derived*>);   // not down
    static_assert(std::is_convertible_v<int, double>);

    std::cout << "is_same, is_base_of, is_convertible all hold\n";
    return 0;
}
