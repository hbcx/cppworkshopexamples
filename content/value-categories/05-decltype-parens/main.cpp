// decltype has two behaviours, and the difference is a pair of parentheses.
// decltype(name) gives the DECLARED type of the entity. decltype((expr)) -- with
// extra parentheses, so it is an expression, not a bare name -- gives a type that
// encodes the value category: T& for an lvalue, T&& for an xvalue, T for a
// prvalue. This bites when writing decltype(auto) return types.
#include <iostream>
#include <type_traits>

int main() {
    int x = 0;
    const int cx = 0;
    (void)x;                 // used only inside unevaluated decltype below
    (void)cx;

    std::cout << std::boolalpha;
    // decltype(x): the declared type, int.
    std::cout << "decltype(x) is int:           "
              << std::is_same<decltype(x), int>::value << '\n';           // true
    // decltype((x)): x is an lvalue expression, so int&.
    std::cout << "decltype((x)) is int&:        "
              << std::is_same<decltype((x)), int&>::value << '\n';        // true
    // const is carried through the expression form.
    std::cout << "decltype((cx)) is const int&: "
              << std::is_same<decltype((cx)), const int&>::value << '\n'; // true
    return 0;
}
