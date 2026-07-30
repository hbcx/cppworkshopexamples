#include <any>
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <variant>

// Anti-pattern: any for a CLOSED, known set of types. The value is always one of
// int/double/string, but any erases that, so every read is an any_cast and a
// forgotten type is a runtime miss, not a compile error.
std::string renderAny(const std::any& a) {
    if (a.type() == typeid(int))
        return "int " + std::to_string(std::any_cast<int>(a));
    if (a.type() == typeid(double))
        return "double " + std::to_string(std::any_cast<double>(a));
    if (a.type() == typeid(std::string))
        return "string " + std::any_cast<std::string>(a);
    return "UNHANDLED";   // the compiler cannot warn that this is reachable
}

// Fix: a variant names the closed set, so std::visit must handle every
// alternative -- a missing case fails to compile. No allocation, no any_cast.
using Value = std::variant<int, double, std::string>;
std::string renderVariant(const Value& v) {
    return std::visit([](const auto& x) -> std::string {
        using T = std::decay_t<decltype(x)>;
        if constexpr (std::is_same_v<T, int>)         return "int " + std::to_string(x);
        else if constexpr (std::is_same_v<T, double>) return "double " + std::to_string(x);
        else                                          return "string " + x;
    }, v);
}

int main() {
    std::any a = std::string("hi");
    std::cout << "any:     " << renderAny(a) << '\n';

    Value v = 42;
    std::cout << "variant: " << renderVariant(v) << '\n';
    return 0;
}
