#include <iostream>
#include <string>
#include <variant>

// A variadic class template can inherit from EACH type in the pack. The
// "overloaded" idiom builds one callable from several lambdas: it derives from
// all of them and pulls in each operator() with a pack expansion in a
// using-declaration.
template <class... Fs>
struct overloaded : Fs... {
    using Fs::operator()...;   // C++17 pack expansion in a using-declaration
};
template <class... Fs>
overloaded(Fs...) -> overloaded<Fs...>;   // deduction guide

int main() {
    std::variant<int, double, std::string> v = std::string("hello");

    auto visitor = overloaded{
        [](int i)                { std::cout << "int: " << i << "\n"; },
        [](double d)             { std::cout << "double: " << d << "\n"; },
        [](const std::string& s) { std::cout << "string: " << s << "\n"; }
    };

    std::visit(visitor, v);   // string: hello
    v = 42;
    std::visit(visitor, v);   // int: 42
    v = 3.5;
    std::visit(visitor, v);   // double: 3.5
    return 0;
}
