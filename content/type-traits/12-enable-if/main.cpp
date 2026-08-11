#include <iostream>
#include <string>
#include <type_traits>

// enable_if_t<Cond, T> is T when Cond is true and DOES NOT EXIST when false, which
// removes a template from overload resolution -- the classic pre-concepts SFINAE.
template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
std::string kind(T) { return "integer"; }

template <class T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
std::string kind(T) { return "floating point"; }

int main() {
    std::cout << "kind(42):  " << kind(42) << "\n";     // integer
    std::cout << "kind(3.5): " << kind(3.5) << "\n";    // floating point

    // As a plain trait: enable_if<true, X>::type is X; enable_if<false, X> has no
    // ::type at all, and naming that missing ::type is what drops the overload.
    static_assert(std::is_same_v<std::enable_if_t<true, double>, double>);

    // kind("hi");   // no overload: neither integral nor floating point enables one

    // In C++20 a concept or a requires-clause says this far more clearly -- see the
    // Concepts chapter -- but enable_if is everywhere in pre-C++20 code.
    std::cout << "done\n";
    return 0;
}
