// A forwarding reference (often called a "universal reference") is T&& where T is
// a template parameter DEDUCED from the argument. Unlike a plain rvalue reference,
// it binds to BOTH lvalues and rvalues: for an lvalue T deduces to U& and T&&
// collapses to U&; for an rvalue T deduces to U and T&& stays U&&. So one template
// handles both, remembering which it got.
#include <iostream>
#include <type_traits>
#include <string>
#include <utility>

template <typename T>
void probe(T&& /*x*/) {
    if constexpr (std::is_lvalue_reference_v<T>)
        std::cout << "bound an lvalue (T = U&)\n";
    else
        std::cout << "bound an rvalue (T = U)\n";
}

int main() {
    std::string s = "x";
    probe(s);                 // lvalue -> T = std::string&
    probe(std::string());     // rvalue -> T = std::string
    probe(std::move(s));      // xvalue is an rvalue -> T = std::string
    return 0;
}
