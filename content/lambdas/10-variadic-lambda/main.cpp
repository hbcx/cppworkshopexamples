#include <iostream>
#include <string>
#include <utility>

int main() {
    // A variadic generic lambda takes any number of arguments through auto... and
    // a fold expression combines the pack.
    auto sum = [](auto... xs) { return (xs + ...); };
    std::cout << "sum(1,2,3,4) = " << sum(1, 2, 3, 4) << '\n';   // 10

    auto printAll = [](const auto&... xs) {
        ((std::cout << xs << ' '), ...);   // comma fold: print each, in order
        std::cout << '\n';
    };
    printAll("id", 42, 3.5, "ok");

    // Capturing a pack (C++20): [...ys = std::move(xs)] moves each argument into the
    // closure, so a returned lambda owns them and can be called later.
    auto makeAdder = [](auto... xs) {
        return [...ys = std::move(xs)] { return (ys + ...); };
    };
    auto adder = makeAdder(10, 20, 30);
    std::cout << "captured pack sum: " << adder() << '\n';   // 60
    return 0;
}
