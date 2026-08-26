// Forwarding references work because of REFERENCE COLLAPSING: when references pile
// up through templates or typedefs, they collapse by the rule "rvalue only if
// BOTH are rvalue":
//   T&  &  -> T&      T&  && -> T&
//   T&& &  -> T&      T&& && -> T&&
// A forwarding reference T&& deduces T = U& for an lvalue, so T&& is U& && -> U&;
// and T = U for an rvalue, so T&& stays U&&. We form the four combinations.
#include <iostream>
#include <type_traits>

int main() {
    using L = int&;      // an lvalue reference
    using R = int&&;     // an rvalue reference

    std::cout << std::boolalpha;
    std::cout << "int&  &   collapses to lvalue ref: " << std::is_lvalue_reference_v<L&>  << '\n'; // true
    std::cout << "int&  &&  collapses to lvalue ref: " << std::is_lvalue_reference_v<L&&> << '\n'; // true
    std::cout << "int&& &   collapses to lvalue ref: " << std::is_lvalue_reference_v<R&>  << '\n'; // true
    std::cout << "int&& &&  stays     rvalue ref:    " << std::is_rvalue_reference_v<R&&> << '\n'; // true
    return 0;
}
