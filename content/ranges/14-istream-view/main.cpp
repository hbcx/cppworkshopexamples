#include <iostream>
#include <ranges>
#include <sstream>

int main() {
    // views::istream<T> lazily reads values of type T with operator>>, one at a
    // time, until an extraction fails (end of stream or bad input).
    std::istringstream in{"3 1 4 1 5 9 2 6"};

    int sum = 0;
    for (int x : std::views::istream<int>(in)) sum += x;
    std::cout << "sum of the stream: " << sum << '\n';   // 31

    // It composes like any view: take the first few, then transform them.
    std::istringstream in2{"10 20 30 40 50"};
    std::cout << "first three doubled:";
    for (int x : std::views::istream<int>(in2)
                     | std::views::take(3)
                     | std::views::transform([](int n) { return n * 2; }))
        std::cout << ' ' << x;                            // 20 40 60
    std::cout << '\n';
    return 0;
}
