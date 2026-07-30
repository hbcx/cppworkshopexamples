#include <iostream>
#include <string>
#include <variant>

// The "overloaded" idiom: inherit from a set of lambdas so each handles one
// alternative type, and add a deduction guide so the braces deduce the lambdas.
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

int main() {
    using Value = std::variant<int, double, std::string>;

    // Per-type handlers instead of one generic body -- each alternative gets its
    // own line, and std::visit still checks that every alternative is covered.
    auto render = overloaded{
        [](int i) { std::cout << "int: " << i << '\n'; },
        [](double d) { std::cout << "double: " << d << '\n'; },
        [](const std::string& s) { std::cout << "string: " << s << '\n'; },
    };

    for (const Value& v : {Value{7}, Value{2.5}, Value{std::string("hi")}}) {
        std::visit(render, v);
    }
    return 0;
}
