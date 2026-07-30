#include <iostream>
#include <string>
#include <type_traits>
#include <variant>

using Value = std::variant<int, double, std::string>;

// Anti-pattern: dispatching by hand with a holds_alternative / get ladder. It
// works, but nothing checks it is exhaustive -- forget an alternative and that
// case silently falls through.
std::string renderLadder(const Value& v) {
    if (std::holds_alternative<int>(v))
        return "int " + std::to_string(std::get<int>(v));
    if (std::holds_alternative<double>(v))
        return "double " + std::to_string(std::get<double>(v));
    // whoops: the std::string case is missing -- and the compiler does not care
    return "UNHANDLED";
}

// Fix: std::visit dispatches to every alternative and will NOT compile if one is
// unhandled, so a missing case is a build error, not a runtime miss.
std::string renderVisit(const Value& v) {
    return std::visit([](const auto& x) -> std::string {
        using T = std::decay_t<decltype(x)>;
        if constexpr (std::is_same_v<T, int>)         return "int " + std::to_string(x);
        else if constexpr (std::is_same_v<T, double>) return "double " + std::to_string(x);
        else                                          return "string " + x;
    }, v);
}

int main() {
    Value v = std::string("hi");
    std::cout << "ladder: " << renderLadder(v) << '\n';   // UNHANDLED -- the bug
    std::cout << "visit:  " << renderVisit(v) << '\n';    // string hi
    return 0;
}
