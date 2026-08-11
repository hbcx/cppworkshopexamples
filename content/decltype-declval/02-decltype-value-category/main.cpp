#include <iostream>
#include <type_traits>
#include <utility>

int main() {
    int x = 0;
    const int cx = 0;
    int& rx = x;
    (void) cx; (void) rx;

    // Rule 1: decltype of a bare NAME (id-expression) is its DECLARED type.
    static_assert(std::is_same<decltype(x),  int>::value,        "x is int");
    static_assert(std::is_same<decltype(cx), const int>::value,  "cx is const int");
    static_assert(std::is_same<decltype(rx), int&>::value,       "rx is int& as declared");

    // Rule 2: decltype of a compound EXPRESSION adds a reference by value
    // category: lvalue -> T&, xvalue -> T&&, prvalue -> T.
    static_assert(std::is_same<decltype(x + 0),         int>::value,   "x+0 is a prvalue -> int");
    static_assert(std::is_same<decltype(std::move(x)),  int&&>::value, "move(x) is an xvalue -> int&&");

    // The trap: extra parentheses make x an lvalue EXPRESSION, not a bare name,
    // so decltype((x)) is int&, not int.
    static_assert(std::is_same<decltype(x),   int>::value,  "decltype(x)   is int");
    static_assert(std::is_same<decltype((x)), int&>::value, "decltype((x)) is int& -- the trap");

    std::cout << "all decltype value-category rules hold\n";
    return 0;
}
