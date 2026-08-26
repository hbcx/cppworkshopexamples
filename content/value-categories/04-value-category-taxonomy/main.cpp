// C++ has three primary value categories:
//   lvalue  -- names something with identity (a variable, a[i], *p)
//   prvalue -- a pure temporary being computed (a literal, a + b, make())
//   xvalue  -- an "expiring" object: an lvalue cast to rvalue, e.g. std::move(x)
// The composites: glvalue = lvalue or xvalue (has identity); rvalue = prvalue or
// xvalue (may be moved from). decltype((expr)) reveals the category: T& for an
// lvalue, T&& for an xvalue, plain T for a prvalue.
#include <iostream>
#include <type_traits>
#include <utility>

template <typename Expr>
const char* category() {
    if constexpr (std::is_lvalue_reference_v<Expr>)      return "lvalue";
    else if constexpr (std::is_rvalue_reference_v<Expr>) return "xvalue";
    else                                                 return "prvalue";
}
#define CATEGORY_OF(expr) category<decltype((expr))>()

int main() {
    int x = 0;
    int arr[3] = {1, 2, 3};
    (void)x;                 // used only inside unevaluated decltype below
    (void)arr;

    std::cout << "x         -> " << CATEGORY_OF(x) << '\n';            // lvalue
    std::cout << "arr[1]    -> " << CATEGORY_OF(arr[1]) << '\n';       // lvalue
    std::cout << "x + 1     -> " << CATEGORY_OF(x + 1) << '\n';        // prvalue
    std::cout << "42        -> " << CATEGORY_OF(42) << '\n';           // prvalue
    std::cout << "move(x)   -> " << CATEGORY_OF(std::move(x)) << '\n'; // xvalue
    return 0;
}
