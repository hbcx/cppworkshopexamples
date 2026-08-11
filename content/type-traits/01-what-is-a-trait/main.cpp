#include <iostream>
#include <type_traits>

int main() {
    std::cout << std::boolalpha;

    // A VALUE trait derives from integral_constant and exposes a ::value; the _v
    // helper is the shorthand.
    std::cout << "is_integral<int>::value = " << std::is_integral<int>::value << "\n";   // true
    std::cout << "is_integral_v<double>   = " << std::is_integral_v<double> << "\n";      // false
    static_assert(std::is_integral<int>::value);
    static_assert(std::is_integral_v<long>);              // _v is the shorthand for ::value

    // A TYPE trait exposes a ::type; the _t helper is the shorthand.
    using A = std::remove_reference<int&>::type;          // int
    using B = std::remove_reference_t<int&>;              // int
    static_assert(std::is_same_v<A, int>);
    static_assert(std::is_same_v<B, int>);

    // true_type and false_type are the two fixed boolean traits.
    static_assert(std::true_type::value == true);
    static_assert(std::false_type::value == false);

    std::cout << "trait basics hold\n";
    return 0;
}
