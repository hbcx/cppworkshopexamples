#include <concepts>
#include <iostream>
#include <type_traits>

// A concept is a named compile-time bool -- and traits are where those bools come
// from. You can define a concept directly from traits; conjunction_v combines them
// like && (and short-circuits), which is the trait-native way to write the "and".
template <class T>
concept SignedInteger = std::conjunction_v<std::is_integral<T>, std::is_signed<T>>;

// The standard concepts are built the same way: std::integral is essentially
// std::is_integral_v, std::same_as wraps std::is_same, and so on.
static_assert(std::integral<int> == std::is_integral_v<int>);
static_assert(std::same_as<int, int> == std::is_same_v<int, int>);

template <SignedInteger T>
T negate(T x) { return -x; }

int main() {
    std::cout << negate(5) << "\n";        // -5, int is a SignedInteger
    // negate(5u);                         // rejected: unsigned is not signed
    // negate(3.0);                        // rejected: double is not integral

    std::cout << std::boolalpha;
    std::cout << "SignedInteger<int>      = " << SignedInteger<int> << "\n";        // true
    std::cout << "SignedInteger<unsigned> = " << SignedInteger<unsigned> << "\n";   // false
    std::cout << "SignedInteger<double>   = " << SignedInteger<double> << "\n";     // false
    return 0;
}
