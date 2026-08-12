#include <iostream>
#include <array>

int main() {
    // A lambda is implicitly constexpr when its body allows it (C++17), so it can
    // run at compile time -- here checked by static_assert.
    constexpr auto square = [](int x) { return x * x; };
    static_assert(square(9) == 81, "runs at compile time");

    // An immediately-invoked constexpr lambda can build a constant table.
    constexpr std::array<int, 5> squares = [] {
        std::array<int, 5> a{};
        for (int i = 0; i < 5; ++i) a[i] = i * i;
        return a;
    }();

    std::cout << "compile-time table: ";
    for (int x : squares) std::cout << x << ' ';
    std::cout << '\n';

    // A consteval lambda (C++20) MUST run at compile time -- a run-time call is an
    // error, which is a stronger guarantee than constexpr.
    constexpr auto cube = [](int x) consteval { return x * x * x; };
    constexpr int c = cube(3);
    std::cout << "consteval cube(3) = " << c << '\n';   // 27
    return 0;
}
