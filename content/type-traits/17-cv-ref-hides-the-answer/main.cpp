#include <iostream>
#include <type_traits>

int main() {
    std::cout << std::boolalpha;

    // A trait sees the EXACT type, cv and reference included. These surprise people:
    static_assert(!std::is_same_v<const int&, int>);   // different types
    static_assert(!std::is_integral_v<int&>);          // a reference is not integral
    static_assert(!std::is_const_v<const int&>);       // the REFERENCE is not const at top level

    // Fix: normalise first. remove_cvref (C++20) strips cv and reference; decay
    // does that plus array/function decay.
    static_assert(std::is_same_v<std::remove_cvref_t<const int&>, int>);
    static_assert(std::is_integral_v<std::remove_cvref_t<int&>>);
    static_assert(std::is_same_v<std::decay_t<const int&>, int>);

    std::cout << "raw     is_same<const int&, int> = "
              << std::is_same_v<const int&, int> << "\n";                               // false
    std::cout << "decayed is_same<..., int>        = "
              << std::is_same_v<std::remove_cvref_t<const int&>, int> << "\n";          // true
    std::cout << "done\n";
    return 0;
}
