#include <iostream>
#include <string>
#include <variant>
#include <vector>

int main() {
    using Value = std::variant<int, double, std::string>;

    // std::visit calls one callable with whichever alternative is active. A
    // generic lambda handles every type in one body; the call is exhaustive and
    // checked at compile time -- add an alternative the lambda cannot handle and
    // it will not compile.
    auto show = [](const Value& v) {
        std::visit([](const auto& x) { std::cout << "value: " << x << '\n'; }, v);
    };

    std::vector<Value> values = {42, 3.14, std::string("hello")};
    for (const Value& v : values) {
        show(v);
    }
    return 0;
}
